set -x

OPT=/usr/local/opt

if $# -eq 1; then
	case $(uname -s) in
		Darwin)
			TARGET="darwin_$(uname -r)-$(uname -m)"
			;;
		Linux)
			. /etc/os-release
			TARGET=${ID}_${VERSION_ID%.*}-$(uname -m)
			;;
		*)
			echo "ERROR: $(uname -s) is not available for fast install. Use build.sh instead."
			exit 1
			;;
	esac
	aws s3 ls s3://install.gridlabd.us | grep $TARGET
else
	mkdir -p $OPT/gridlabd
	cd $OPT/gridlabd
	curl -sL https://install.gridlabd.us/$1.tarz | tar xz
	sh $1/share/gridlabd/setup.sh
	ln -sf $1 current
	ln -sF $OPT/current/bin/gridlabd /usr/local/bin/gridlabd
	/usr/local/bin/gridlabd --version=install
fi