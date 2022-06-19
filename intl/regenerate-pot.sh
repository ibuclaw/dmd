#!/bin/bash

# Always operate in the C locale.
LANG=C
LANGUAGE=C
LC_ALL=C
export LANG LANGUAGE LC_ALL

srcdir=../compiler/src
pwd=`pwd`

set -e

# Create temporary directory for scratch files.
T=exg$$.d
mkdir $T
trap "rm -r $T" 0

tmp_pot=$pwd/$T/tmp.pot
po_sources=$pwd/$T/po-d-sources

pushd $srcdir
find dmd -type f -name "*.d" > $po_sources
popd

# Extract all strings from diagnostic messages
xgettext \
    --default-domain=dmd \
    --copyright-holder="D Language Foundation" \
    --msgid-bugs-address="https://issues.dlang.org/" \
    --directory=$srcdir \
    --files-from=$po_sources \
    --from-code=utf-8 \
    --language=c \
    --no-wrap \
    `grep -v '#' keywords.list` \
    -o $tmp_pot

sed "s:$srcdir/::g;s:$pwd/::g;" < $tmp_pot > dmd.pot

# Merge strings with existing .po files
# Create a new translation by copying dmd.pot, i.e: `cp dmd.pot de.po`
for lang in `find . -name '*.po'`; do
    msgmerge --no-wrap $lang dmd.pot > $tmp_pot
    cp $tmp_pot $lang
done

# TODO: Install
# msgfmt --statistics de.po -o ../generated/linux/release/64/locale/de/LC_MESSAGES/dmd.mo 

