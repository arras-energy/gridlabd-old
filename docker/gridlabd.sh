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
autoreconf -isf 
./configure 
make -j30 install
if [ -x "$INSTALL/bin/gridlabd-version" -a "$TEST" == "yes" ]; then
	log "ACTIVATE: automatic"
	run $INSTALL/bin/gridlabd version set "$VERSION"
elif [ "$LINK" == "yes" ]; then
	log "ACTIVATE: manual"
	[ ! -L "$PREFIX/opt/gridlabd/current" ] && run sudo rm -f "$PREFIX/opt/gridlabd/current"
	run sudo ln -sf "$INSTALL" "$PREFIX/opt/gridlabd/current"
	for dir in bin lib include share; do
		run sudo ln -sf $PREFIX/opt/gridlabd/current/$dir/gridlabd $PREFIX/$dir/gridlabd
	done
	run sudo ln -sf $PREFIX/opt/gridlabd/current/bin/gridlabd.bin $PREFIX/bin/gridlabd.bin
fi
make validate