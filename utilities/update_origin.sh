#/bin/bash
ORIGIN=$(git remote -v show | head -n 1 | cut -f1)
COMMIT=$(git log -n 1 | head -n 1 | cut -f2 -d' ')
URL=$(git remote get-url $ORIGIN | sed -e 's/\.git$//')
echo "# $URL/commits/$COMMIT" 
git status -s | sed -e '1,$s/^/# /'
git diff
