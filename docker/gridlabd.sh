REPO=${REPO:-https://github.com/slacgismo/gridlabd}
BRANCH=${BRANCH:-master}
echo "
#####################################
# DOCKER BUILD
#   gridlabd <- $REPO/$BRANCH
#####################################
"

# gridlabd source
cd /usr/local/src
git clone $REPO gridlabd -b $BRANCH
if [ ! -d /usr/local/src/gridlabd ]; then
	echo "ERROR: unable to download $REPO/$BRANCH"
	exit 1
fi

cd gridlabd 
bash ./install.sh ${INSTALL_OPTIONS:---verbose}

