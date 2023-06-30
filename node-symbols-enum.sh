#!/bin/sh

head -n128 subprojects/tree-sitter-nix/src/tree-sitter-nix/src/parser.c | tail -n110 >src/node-symbols-enum.h
