#/bin/bash
if [ -d .git ]; then

    # git info is available
    ORIGIN=$(git remote -v show | head -n 1 | cut -f1)
    COMMIT=$(git log -n 1 | head -n 1 | cut -f2 -d' ')
    URL=$(git remote get-url $ORIGIN | sed -e 's/\.git$//')
    echo "# $URL/commits/$COMMIT" 
    git status -s | sed -e '1,$s/^/# /'
    git diff

else

    # no git info available
    echo "No origin info available (no git repository)"
fi

