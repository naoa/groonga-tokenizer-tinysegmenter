/*
  Copyright(C) 2014 Naoya Murakami <naoya@createfield.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License version 2.1 as published by the Free Software Foundation.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
  02110-1301  USA
*/

#include <groonga/tokenizer.h>

#include <string.h>
#include <ctype.h>

#include <iostream>
#include <string>
#include <vector>
#include "tinysegmenter.hpp"

using namespace std;

#ifdef __GNUC__
#  define GNUC_UNUSED __attribute__((__unused__))
#else
#  define GNUC_UNUSED
#endif

#define GRN_STRING_ENABLE_NORMALIZER_FILTER (0x01<<5)

typedef struct {
  grn_tokenizer_token token;
  grn_tokenizer_query *query;
  vector<string> parsed_vector;
  unsigned int next;
  unsigned int end;
} grn_tinysegmenter_tokenizer;

static grn_obj *
tinysegmenter_init(grn_ctx *ctx, int nargs, grn_obj **args, grn_user_data *user_data)
{
  grn_tokenizer_query *query;
  unsigned int normalize_flags = GRN_STRING_ENABLE_NORMALIZER_FILTER;
  const char *normalized;
  unsigned int normalized_length_in_bytes;
  grn_tinysegmenter_tokenizer *tokenizer;
  TinySegmenter segmenter;

  query = grn_tokenizer_query_open(ctx, nargs, args, normalize_flags);
  if (!query) {
    return NULL;
  }

  tokenizer = static_cast<grn_tinysegmenter_tokenizer *>(
    GRN_PLUGIN_MALLOC(ctx, sizeof(grn_tinysegmenter_tokenizer)));
  if (!tokenizer) {
    GRN_PLUGIN_ERROR(ctx,GRN_NO_MEMORY_AVAILABLE,
                     "[tokenizer][tinysegmenter] "
                     "memory allocation to grn_tinysegmenter_tokenizer failed");
    grn_tokenizer_query_close(ctx, query);
    return NULL;
  }
  user_data->ptr = tokenizer;
  grn_tokenizer_token_init(ctx, &(tokenizer->token));
  tokenizer->query = query;
  grn_string_get_normalized(ctx, tokenizer->query->normalized_query,
                            &normalized, &normalized_length_in_bytes,
                            NULL);

  tokenizer->parsed_vector = segmenter.segment(normalized);
  tokenizer->next = 0;
  tokenizer->end = tokenizer->parsed_vector.size();
  
  return NULL;
}

static grn_obj *
tinysegmenter_next(grn_ctx *ctx, GNUC_UNUSED int nargs, GNUC_UNUSED grn_obj **args,
                   grn_user_data *user_data)
{
  grn_tinysegmenter_tokenizer *tokenizer =
    static_cast<grn_tinysegmenter_tokenizer *>(user_data->ptr);

  grn_tokenizer_status status;

  if (tokenizer->next == tokenizer->end - 1) {
    status = GRN_TOKENIZER_LAST;
  } else {
    status = GRN_TOKENIZER_CONTINUE;
  }
  grn_tokenizer_token_push(ctx, &(tokenizer->token),
                           tokenizer->parsed_vector[tokenizer->next].c_str(),
                           tokenizer->parsed_vector[tokenizer->next].length(),
                           status);
  tokenizer->next++;

  return NULL;
}

static grn_obj *
tinysegmenter_fin(grn_ctx *ctx, GNUC_UNUSED int nargs, GNUC_UNUSED grn_obj **args,
                  grn_user_data *user_data)
{
  grn_tinysegmenter_tokenizer *tokenizer =
    static_cast<grn_tinysegmenter_tokenizer *>(user_data->ptr);
  if (!tokenizer) {
    return NULL;
  }
  grn_tokenizer_query_close(ctx, tokenizer->query);
  grn_tokenizer_token_fin(ctx, &(tokenizer->token));
  GRN_PLUGIN_FREE(ctx,tokenizer);
  return NULL;
}

extern "C" {

grn_rc
GRN_PLUGIN_INIT(grn_ctx *ctx)
{
  return ctx->rc;
}

grn_rc
GRN_PLUGIN_REGISTER(grn_ctx *ctx)
{
  grn_rc rc;
  rc = grn_tokenizer_register(ctx, "TokenTinySegmenter", -1,
                              tinysegmenter_init, tinysegmenter_next, tinysegmenter_fin);
  return rc;
}

grn_rc
GRN_PLUGIN_FIN(grn_ctx *ctx)
{
  return GRN_SUCCESS;
}

}

