#!/bin/bash

if [ -z "$1" ]; then
    echo "Fehler: Bitte gib eine Commit-Nachricht an."
    echo "Nutzung: ./gpush.sh \"deine nachricht\""
    exit 1
fi

git add . && git commit -m "$1" && git pull --rebase && git push