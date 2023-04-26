set -x
apt-get update
apt-get upgrade -y
apt-get install curl -y
export REPO=dchassin/gridlabd
export BRANCH=develop-fix-install
export GRIDLABD_ORIGIN=$REPO/$BRANCH
cat /source/setup.sh | sh
git clone https://github.com/$REPO -b $BRANCH --depth 1 /gridlabd
cd /gridlabd
cat /source/build.sh | sh
echo "Starting interactive shell. Use Ctrl-D to exit"
bash
