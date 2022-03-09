FROM debian:11

RUN apt-get -q update

RUN apt-get install tzdata -y
RUN  apt-get install curl -y
RUN apt-get install apt-utils -y

RUN apt-get -q install software-properties-common -y
RUN apt install build-essential zlib1g-dev libncurses5-dev libgdbm-dev libnss3-dev libssl-dev libreadline-dev libffi-dev -y



RUN apt-get -q install git -y
RUN apt-get -q install unzip -y
RUN apt-get -q install autoconf -y
RUN apt-get -q install libtool -y
RUN apt-get -q install g++ -y
RUN apt-get -q install cmake -y 
RUN apt-get -q install flex -y
RUN apt-get -q install bison -y
RUN apt-get -q install libcurl4-gnutls-dev -y
RUN apt-get -q install subversion -y
RUN apt-get -q install util-linux -y
RUN apt-get install liblzma-dev -y
RUN apt-get install libbz2-dev -y
RUN apt-get install libncursesw5-dev -y
RUN apt-get install xz-utils -y

# install gdal 
RUN apt-get install libgdal-dev -y

WORKDIR /usr/local/src

RUN curl https://www.python.org/ftp/python/3.9.6/Python-3.9.6.tgz | tar xz
	# tar xzf Python-3.9.6.tgz 
WORKDIR /usr/local/src/Python-3.9.6

RUN ./configure --prefix=/usr/local --enable-shared --enable-optimizations --with-system-ffi --with-computed-gotos --enable-loadable-sqlite-extensions CFLAGS="-fPIC"

RUN make -j 6
RUN make altinstall
RUN /sbin/ldconfig /usr/local/lib
RUN	ln -sf /usr/local/bin/python3.9 /usr/local/bin/python3
RUN	ln -sf /usr/local/bin/python3.9-config /usr/local/bin/python3-config
RUN	ln -sf /usr/local/bin/pydoc3.9 /usr/local/bin/pydoc
RUN	ln -sf /usr/local/bin/idle3.9 /usr/local/bin/idle
RUN	ln -sf /usr/local/bin/pip3.9 /usr/local/bin/pip3

# config gdal
RUN export CPLUS_INCLUDE_PATH=/usr/include/gdal

RUN	/usr/local/bin/python3 -m pip install --upgrade pip
RUN	/usr/local/bin/python3 -m pip install matplotlib Pillow pandas numpy networkx pytz pysolar PyGithub scikit-learn xlrd boto3
RUN	/usr/local/bin/python3 -m pip install IPython censusdata
# RUN /usr/local/bin/python3 -m pip install pygdal==3.2.2

WORKDIR /usr/local/src


# RUN	/usr/local/bin/python3 -m pip install geopandas

RUN git clone https://github.com/slacgismo/gridlabd.git

WORKDIR /usr/local/src/gridlabd
# COPY ./requirements.txt ./requirements.txt
# COPY ./gldcore/geodata/geodata_utility.py ./gldcore/geodata/geodata_utility.py 
# COPY ./gldcore/geodata/requirements.txt ./gldcore/geodata/requirements.txt
# COPY ./gldcore/scripts/requirements.txt ./gldcore/scripts/requirements.txt
COPY ./install.sh ./install.sh
RUN autoreconf -isf && ./configure
RUN make -j6 system