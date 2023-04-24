apt update 
apt install curl -y

export GRIDLABD_ORG=dchassin
export GRIDLABD_REPO=gridlabd
export GRIDLABD_BRANCH=develop-fix-install

curl -sL https://raw.githubusercontent.com/${GRIDLABD_ORG}/${GRIDLABD_REPO}/${GRIDLABD_BRANCH}/setup.sh | sh

. $HOME/.venv/gridlabd/bin/activate

apt install git -y
git clone https://github.com/$GRIDLABD_ORG/$GRIDLABD_REPO -b $GRIDLABD_BRANCH --depth 1 gridlabd

cd /gridlabd
autoreconf -isf
./configure
make -j$(($(nproc)*3)) system
gridlabd -T 0 --validate

echo "Starting interactive shell. Use Ctrl-D to exit."
bash
