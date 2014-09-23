# TinySegmenter tokenizer plugin for Groonga

## Tokenizer

### TokenTinySegmenter

[TinySegmenter](http://chasen.org/~taku/software/TinySegmenter/)用の学習モデルを自作するための[TinySegmenterMaker](https://github.com/shogo82148/TinySegmenterMaker)を使って生成したC++用のライブラリ``tinysegmenter.hpp``を使って分かち書きをします。

TinySegmenterは、機械学習のみを使って分かち書きを行います。  
TinySegmenterは、形態素解析用の辞書を持たないため、サイズを非常にコンパクトに抑えることができます。  
学習モデルは、オリジナルのTinySegmenterと同様のものを用いています。  
この学習モデルは日本語の新聞記事向けに学習されています。  
TinySegmenterMakerを使えば、分かち書き済みテキストを用意するだけで簡単に自作の学習モデルを作ることができます。  
``tokenizers/tinysegmenter.hpp``を置き換えてソースをビルドすれば自作の学習モデルを利用することができます。

```
> tokenize TokenTinySegmenter "今日は雨です。"
[
  [
    0,
    0.0,
    0.0
  ],
  [
    {
      "value": "今日",
      "position": 0
    },
    {
      "value": "は",
      "position": 1
    },
    {
      "value": "雨",
      "position": 2
    },
    {
      "value": "です",
      "position": 3
    },
    {
      "value": "。",
      "position": 4
    }
  ]
]
```

## Install

Install ``groonga-tokenizer-tinysegmenter`` package:

### CentOS

* CentOS6

```
% sudo yum localinstall -y http://packages.createfield.com/centos/6/groonga-tokenizer-tinysegmenter-1.0.0-1.el6.x86_64.rpm
```

* CentOS7

```
% sudo yum localinstall -y http://packages.createfield.com/centos/7/groonga-tokenizer-tinysegmenter-1.0.0-1.el7.centos.x86_64.rpm
```

### Fedora

* Fedora 20

```
% sudo yum localinstall -y http://packages.createfield.com/fedora/20/groonga-tokenizer-tinysegmenter-1.0.0-1.fc20.x86_64.rpm
```

* Fedora 21

```
% sudo yum localinstall -y http://packages.createfield.com/fedora/21/groonga-tokenizer-tinysegmenter-1.0.0-1.fc21.x86_64.rpm
```

### Debian GNU/LINUX

* wheezy

```
% wget http://packages.createfield.com/debian/wheezy/groonga-tokenizer-tinysegmenter_1.0.0-1_amd64.deb
% sudo dpkg -i groonga-tokenizer-tinysegmenter_1.0.0-1_amd64.deb
```

* jessie

```
% wget http://packages.createfield.com/debian/jessie/groonga-tokenizer-tinysegmenter_1.0.0-1_amd64.deb
% sudo dpkg -i groonga-tokenizer-tinysegmenter_1.0.0-1_amd64.deb
```


### Ubuntu

* precise

```
% wget http://packages.createfield.com/ubuntu/precise/groonga-tokenizer-tinysegmenter_1.0.0-1_amd64.deb
% sudo dpkg -i groonga-tokenizer-tinysegmenter_1.0.0-1_amd64.deb
```

* trusty

```
% wget http://packages.createfield.com/ubuntu/trusty/groonga-tokenizer-tinysegmenter_1.0.0-1_amd64.deb
% sudo dpkg -i groonga-tokenizer-tinysegmenter_1.0.0-1_amd64.deb
```

### Source install

Build this tokenizer.

    % sh autogen.sh
    % ./configure
    % make
    % sudo make install

## Dependencies

* Groonga >= 4.0.5

Install ``groonga-devel`` in CentOS/Fedora. Install ``libgroonga-dev`` in Debian/Ubuntu.

See http://groonga.org/docs/install.html

## Usage

Firstly, register `tokenizers/tinysegmenter`

Groonga:

    % groonga db
    > register tokenizers/tinysegmenter
    > table_create Diaries TABLE_HASH_KEY INT32
    > column_create Diaries body COLUMN_SCALAR TEXT
    > table_create Terms TABLE_PAT_KEY ShortText --default_tokenizer TokenTinySegmenter
    > column_create Terms diaries_body COLUMN_INDEX|WITH_POSITION Diaries body

Mroonga:

    mysql> use db;
    mysql> CREATE TABLE `temp` (id INT NOT NULL) ENGINE=mroonga DEFAULT CHARSET=utf8;
    mysql> DROP TABLE `temp`;
    mysql> select mroonga_command("register tokenizers/tinysegmenter");
    mysql> CREATE TABLE `Diaries` (
        -> id INT NOT NULL,
        -> body TEXT NOT NULL,
        -> PRIMARY KEY (id) USING HASH,
        -> FULLTEXT INDEX (body) COMMENT 'parser "TokenTinySegmenter"'
        -> ) ENGINE=mroonga DEFAULT CHARSET=utf8;

Rroonga:

    irb --simple-prompt -rubygems -rgroonga
    >> Groonga::Context.default_options = {:encoding => :utf8}   
    >> Groonga::Database.create(:path => "/tmp/db")
    >> Groonga::Plugin.register(:path => "/usr/lib/groonga/plugins/tokenizers/tinysegmenter.so")
    >> Groonga::Schema.create_table("Diaries",
    ?>                              :type => :hash,
    ?>                              :key_type => :integer32) do |table|
    ?>   table.text("body")
    >> end
    >> Groonga::Schema.create_table("Terms",
    ?>                              :type => :patricia_trie,
    ?>                              :normalizer => :NormalizerAuto,
    ?>                              :default_tokenizer => "TokenTinySegmenter") do |table|
    ?>   table.index("Diaries.body")
    >> end
    
## Author

* Naoya Murakami <naoya@createfield.com>

## License

LGPL 2.1. See COPYING for details.
This program is the same license as Groonga.

This program includes the original TinySegmenter model.  
TinySegmenter is licensed under New BSD License. 

http://chasen.org/~taku/software/TinySegmenter/

// (c) 2008 Taku Kudo <taku@chasen.org>  
// TinySegmenter is freely distributable under the terms of a new BSD licence.  
// For details, see http://chasen.org/~taku/software/TinySegmenter/LICENCE.txt

``tokenizers/tinysegmenter.hpp`` is created by TinySegmenterMaker.

https://github.com/shogo82148/TinySegmenterMaker

TinySegmenterMaker is provided by [shogo81248](https://github.com/shogo82148).

