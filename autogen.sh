#!/usr/bin/env bash

libtoolize --copy --force --install
aclocal --force
autoconf --force
automake --add-missing --copy --force
