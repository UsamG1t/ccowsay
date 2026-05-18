#!/bin/sh
set -eu
output=$(${CCOWSAY_TEST_COWSAY:-./ccowsay} --list)
printf '%s\n' "$output" | grep '^default[[:space:]]'
printf '%s\n' "$output" | grep '^dragon[[:space:]]'
