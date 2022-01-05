FROM centos:8
# Install needed system tools
RUN yum install git -y
# RUN yum clean all
# RUN yum groupinstall "Development Tools" -y
# RUN yum install cmake -y 
# RUN yum install ncurses-devel -y
# RUN yum install epel-release -y
# RUN yum install curl-devel -y
# RUN yum install which -y
# RUN yum install svn -y





# RUN yum install 'dnf-command(config-manager)' -y

# #RUN yum config-manager --set-enabled PowerTools # error
# #RUN yum install armadillo-devel -y #error

# # change directory
# WORKDIR "/usr/local/src"
# RUN yum install gcc openssl-devel bzip2-devel libffi-devel zlib-devel  xz-devel  -y

# Install python 3.9.6
# RUN curl https://www.python.org/ftp/python/3.9.6/Python-3.9.6.tgz > Python-3.9.6.tgz
# RUN tar xzf Python-3.9.6.tgz 
# WORKDIR "/usr/local/src/Python-3.9.6"
# RUN ./configure --prefix=/usr/local --enable-optimizations --with-system-ffi --with-computed-gotos --enable-loadable-sqlite-extensions CFLAGS="-fPIC"
# RUN make -j 10
# RUN make altinstall
# RUN /sbin/ldconfig /usr/local/lib
# RUN ln -sf /usr/local/bin/python3.9 /usr/local/bin/python3
# RUN ln -sf /usr/local/bin/python3.9-config /usr/local/bin/python3-config
# RUN ln -sf /usr/local/bin/pydoc3.9 /usr/local/bin/pydoc
# RUN ln -sf /usr/local/bin/idle3.9 /usr/local/bin/idle
# RUN ln -sf /usr/local/bin/pip3.9 /usr/local/bin/pip3
# RUN pip3 install --upgrade pip

# RUN /usr/local/bin/python3 -m pip install matplotlib Pillow pandas numpy networkx pytz pysolar PyGithub scikit-learn xlrd boto3
# RUN /usr/local/bin/python3 -m pip install IPython censusdata
# # mac m1
# # RUN /usr/local/bin/python3 -m pip install shapely 
# WORKDIR /usr/local/src
# RUN rm -f Python-3.9.6.tgz

# RUN export PYTHONPATH="."
# RUN export PATH=/usr/local/bin:$PATH
# RUN export MAKEFLAGS=-j20
# RUN export PYTHONSETUPFLAGS="-j 20"
# clone gridlabd 
WORKDIR /usr/local/src
RUN git clone https://github.com/slacgismo/gridlabd
WORKDIR /usr/local/src/gridlabd
RUN rm -rf ./build-aux/setup-Linux-centos-8.sh
COPY ./build-aux/setup-Linux-centos-8.sh ./build-aux/setup-Linux-centos-8.sh
COPY ./requirements.txt ./requirements.txt
COPY ./gldcore/scripts/autotest/test_matrix.glm ./gldcore/scripts/autotest/test_matrix.glm
RUN   chmod +rxw ./gldcore/scripts/autotest/test_matrix.glm
COPY ./gldcore/scripts/autotest/test_matrix_linalg.glm ./gldcore/scripts/autotest/test_matrix_linalg.glm
RUN   chmod +rxw ./gldcore/scripts/autotest/test_matrix_linalg.glm
COPY ./gldcore/scripts/autotest/test_matrix_matlib.glm ./gldcore/scripts/autotest/test_matrix_matlib.glm
RUN   chmod +rxw ./gldcore/scripts/autotest/test_matrix_matlib.glm
COPY ./gldcore/scripts/autotest/test_matrix_matrix.glm ./gldcore/scripts/autotest/test_matrix_matrix.glm
RUN   chmod +rxw ./gldcore/scripts/autotest/test_matrix_matrix.glm
COPY ./gldcore/scripts/autotest/test_matrix_random.glm ./gldcore/scripts/autotest/test_matrix_random.glm
RUN   chmod +rxw ./gldcore/scripts/autotest/test_matrix_random.glm
RUN chmod +rxw ./requirements.txt
RUN chmod +rxw ./build-aux/setup-Linux-centos-8.sh


COPY ./install.sh ./install.sh

# test install.sh
# RUN ./install.sh -v -t --parallel

# WORKDIR "/usr/local/src/gridlabd"
# RUN autoreconf -isf && ./configure
# RUN make -j6 system