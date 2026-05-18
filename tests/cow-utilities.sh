#!/bin/sh
set -eu

COWSAY=${CCOWSAY_TEST_COWSAY:-./ccowsay}
COWTHINK=${CCOWSAY_TEST_COWTHINK:-./cowthink}

$COWSAY --list | while IFS=' 	' read -r cow _; do
    [ -n "$cow" ] || continue
    $COWSAY --cow "$cow" hello | grep '< hello >' >/dev/null
    $COWTHINK --cow "$cow" hello | grep '( hello )' >/dev/null
done
