# Notes: docker build -t sl7-ssh . && docker run --env="DISPLAY" --volume="$HOME/.Xauthority:/root/.Xauthority:rw" -p 2222:22 -d --name sl7-container sl7-ssh && docker ps --all
# dockerclean && docker build -t smsimulator . && docker run -d --name smsimulator-container smsimulator sleep infinity && docker ps --all && docker exec -it smsimulator-container /bin/bash
# alias dockerclean='docker stop (docker ps -q); docker rm (docker ps -a -q); docker ps --all' 

# Use the Scientific Linux 7 base image
    FROM sl:7

# Quality of life
    RUN echo "alias ll='ls -lah'" >> /root/.bashrc
    RUN echo "alias ..='cd ..'" >> /root/.bashrc

# Dependencies and nice-to-have tools
    RUN yum update -y && yum install -y vim wget emacs tmux libtool automake gcc-c++ gcc

# ROOT dependencies
    RUN yum install -y git make binutils libX11-devel libXpm-devel libXft-devel libXext-devel python openssl-devel xrootd-client-devel xrootd-libs-devel

# Manually add cmake v3.8
    RUN mkdir -p /root/cmake_v3.8/cmake_src /root/cmake_v3.8/cmake_build /root/cmake_v3.8/cmake_install
    WORKDIR /root/cmake_v3.8
    RUN wget https://cmake.org/files/v3.8/cmake-3.8.2-Linux-x86_64.sh && chmod +x cmake-3.8.2-Linux-x86_64.sh
    RUN ./cmake-3.8.2-Linux-x86_64.sh --prefix=/root/cmake_v3.8/cmake_install --skip-license
    ENV PATH=/root/cmake_v3.8/cmake_install/bin:$PATH
    RUN echo "export PATH=/root/cmake_v3.8/cmake_install/bin:\$PATH" >> /root/.bashrc

# ROOT dependency for lib/libXMLIO.* and TDOMParser.h
    RUN yum install -y libxml2-devel

# Compile and install root
    RUN mkdir -p /root/root_v5/build /root/root_v5/install
    RUN git clone --branch v5-34-00-patches --depth=1 https://github.com/root-project/root.git /root/root_v5/src
    WORKDIR /root/root_v5/build
    RUN cmake -DCMAKE_INSTALL_PREFIX=../install ../src && cmake --build . -- install -j$(nproc)
    RUN echo "source /root/root_v5/install/bin/thisroot.sh" >> /root/.bashrc

# Manually replicate 'source /root/root_v5/install/bin/thisroot.sh'
    ENV MANPATH=/root/root_v5/install/man:
    ENV LD_LIBRARY_PATH=/root/root_v5/install/lib
    ENV LIBPATH=/root/root_v5/install/lib
    ENV PATH=/root/root_v5/install/bin:/root/cmake_v3.8/cmake_install/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/root/bin
    ENV ROOTSYS=/root/root_v5/install
    ENV DYLD_LIBRARY_PATH=/root/root_v5/install/lib
    ENV PYTHONPATH=/root/root_v5/install/lib
    ENV LOGNAME=root
    ENV SHLIB_PATH=/root/root_v5/install/lib
    ENV CMAKE_PREFIX_PATH=/root/root_v5/install
    ENV _=/usr/bin/env

# Install GEANT4 v10
    RUN mkdir -p /root/geant4_v10/install /root/geant4_v10/build
    RUN git clone --branch geant4-10.5-release --depth=1 https://github.com/Geant4/geant4.git /root/geant4_v10/src
    WORKDIR /root/geant4_v10/build
    RUN cmake -DGEANT4_INSTALL_DATA=ON -DCMAKE_INSTALL_PREFIX=/root/geant4_v10/install /root/geant4_v10/src
    RUN make -j$(nproc) && make install

# Install anaroot v4.5.39
    RUN mkdir /root/anaroot_v4.5.39
    WORKDIR /root/anaroot_v4.5.39
    RUN wget --trust-server-names --content-disposition "https://ribf.riken.jp/RIBFDAQ/index.php?plugin=attach&refer=Tools%2FAnalysis%2FANAROOT%2FInstallation&openfile=anaroot_v4.5.39.tgz"
    RUN tar xvf anaroot_v4.5.39.tgz && rm -f anaroot_v4.5.39.tgz
    WORKDIR /root/anaroot_v4.5.39/anaroot
    RUN aclocal && libtoolize --force && automake -a --add-missing && autoconf
    RUN ./configure --enable-maintainer-mode --prefix=$PWD
    ENV TARTSYS=/root/anaroot_v4.5.39/anaroot
    ENV LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/root/anaroot_v4.5.39/anaroot/lib
    RUN echo "export TARTSYS=/root/anaroot_v4.5.39/anaroot" >> /root/.bashrc
    RUN echo "export LD_LIBRARY_PATH=\$LD_LIBRARY_PATH:/root/anaroot_v4.5.39/anaroot/lib" >> /root/.bashrc
    RUN make install -j$(nproc)

# Install smsimulator v5.5
    RUN mkdir /root/smsimulator
    WORKDIR /root/smsimulator
    RUN wget http://be.nucl.ap.titech.ac.jp/~nebula/simulator/smsimulator5.5.tar.gz
    RUN tar xvf smsimulator5.5.tar.gz
    RUN rm smsimulator5.5.tar.gz
    WORKDIR /root/smsimulator/smsimulator5.5
    
    # GEANT4 env variables
        ENV geant4make_root=/root/geant4_v10/install/share/Geant4-10.5.1/geant4make
        ENV g4sls_sourced_dir=/root/geant4_v10/install/share/Geant4-10.5.1/geant4make
        ENV G4SYSTEM=Linux-g++
        ENV G4INSTALL=/root/geant4_v10/install/share/Geant4-10.5.1/geant4make
        ENV G4INCLUDE=/root/geant4_v10/install/include/Geant4
        ENV PATH=/root/geant4_workdir/bin/Linux-g++:/root/geant4_v10/install/bin:/root/root_v5/install/bin:/root/cmake_v3.8/cmake_install/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/root/bin
        ENV G4LIB=/root/geant4_v10/install/lib64/Geant4-10.5.1
        ENV LD_LIBRARY_PATH=/root/geant4_v10/install/lib64:/root/root_v5/install/lib:/root/anaroot_v4.5.39/anaroot/lib
        ENV G4WORKDIR=/root/geant4_workdir
        ENV G4LIB_BUILD_SHARED=1
        ENV G4NEUTRONHPDATA=/root/geant4_v10/install/share/Geant4-10.5.1/data/G4NDL4.5
        ENV G4LEDATA=/root/geant4_v10/install/share/Geant4-10.5.1/data/G4EMLOW7.7
        ENV G4LEVELGAMMADATA=/root/geant4_v10/install/share/Geant4-10.5.1/data/PhotonEvaporation5.3
        ENV G4RADIOACTIVEDATA=/root/geant4_v10/install/share/Geant4-10.5.1/data/RadioactiveDecay5.3
        ENV G4PARTICLEXSDATA=/root/geant4_v10/install/share/Geant4-10.5.1/data/G4PARTICLEXS1.1
        ENV G4PIIDATA=/root/geant4_v10/install/share/Geant4-10.5.1/data/G4PII1.3
        ENV G4REALSURFACEDATA=/root/geant4_v10/install/share/Geant4-10.5.1/data/RealSurface2.1.1
        ENV G4SAIDXSDATA=/root/geant4_v10/install/share/Geant4-10.5.1/data/G4SAIDDATA2.0
        ENV G4ABLADATA=/root/geant4_v10/install/share/Geant4-10.5.1/data/G4ABLA3.1
        ENV G4INCLDATA=/root/geant4_v10/install/share/Geant4-10.5.1/data/G4INCL1.0
        ENV G4ENSDFSTATEDATA=/root/geant4_v10/install/share/Geant4-10.5.1/data/G4ENSDFSTATE2.2
        ENV G4LIB_USE_ZLIB=1
        ENV G4UI_USE_TCSH=1

        RUN echo "geant4make_root=/root/geant4_v10/install/share/Geant4-10.5.1/geant4make" >> /root/.bashrc
        RUN echo "g4sls_sourced_dir=/root/geant4_v10/install/share/Geant4-10.5.1/geant4make" >> /root/.bashrc
        RUN echo "G4SYSTEM=Linux-g++" >> /root/.bashrc
        RUN echo "G4INSTALL=/root/geant4_v10/install/share/Geant4-10.5.1/geant4make" >> /root/.bashrc
        RUN echo "G4INCLUDE=/root/geant4_v10/install/include/Geant4" >> /root/.bashrc
        RUN echo "PATH=/root/geant4_workdir/bin/Linux-g++:/root/geant4_v10/install/bin:/root/root_v5/install/bin:/root/cmake_v3.8/cmake_install/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/root/bin" >> /root/.bashrc
        RUN echo "G4LIB=/root/geant4_v10/install/lib64/Geant4-10.5.1" >> /root/.bashrc
        RUN echo "LD_LIBRARY_PATH=/root/geant4_v10/install/lib64:/root/root_v5/install/lib:/root/anaroot_v4.5.39/anaroot/lib" >> /root/.bashrc
        RUN echo "G4WORKDIR=/root/geant4_workdir" >> /root/.bashrc
        RUN echo "G4LIB_BUILD_SHARED=1" >> /root/.bashrc
        RUN echo "G4NEUTRONHPDATA=/root/geant4_v10/install/share/Geant4-10.5.1/data/G4NDL4.5" >> /root/.bashrc
        RUN echo "G4LEDATA=/root/geant4_v10/install/share/Geant4-10.5.1/data/G4EMLOW7.7" >> /root/.bashrc
        RUN echo "G4LEVELGAMMADATA=/root/geant4_v10/install/share/Geant4-10.5.1/data/PhotonEvaporation5.3" >> /root/.bashrc
        RUN echo "G4RADIOACTIVEDATA=/root/geant4_v10/install/share/Geant4-10.5.1/data/RadioactiveDecay5.3" >> /root/.bashrc
        RUN echo "G4PARTICLEXSDATA=/root/geant4_v10/install/share/Geant4-10.5.1/data/G4PARTICLEXS1.1" >> /root/.bashrc
        RUN echo "G4PIIDATA=/root/geant4_v10/install/share/Geant4-10.5.1/data/G4PII1.3" >> /root/.bashrc
        RUN echo "G4REALSURFACEDATA=/root/geant4_v10/install/share/Geant4-10.5.1/data/RealSurface2.1.1" >> /root/.bashrc
        RUN echo "G4SAIDXSDATA=/root/geant4_v10/install/share/Geant4-10.5.1/data/G4SAIDDATA2.0" >> /root/.bashrc
        RUN echo "G4ABLADATA=/root/geant4_v10/install/share/Geant4-10.5.1/data/G4ABLA3.1" >> /root/.bashrc
        RUN echo "G4INCLDATA=/root/geant4_v10/install/share/Geant4-10.5.1/data/G4INCL1.0" >> /root/.bashrc
        RUN echo "G4ENSDFSTATEDATA=/root/geant4_v10/install/share/Geant4-10.5.1/data/G4ENSDFSTATE2.2" >> /root/.bashrc
        RUN echo "G4LIB_USE_ZLIB=1" >> /root/.bashrc
        RUN echo "G4UI_USE_TCSH=1" >> /root/.bashrc

    # smsimulator env variables (source setup.sh)
        ENV SMSIMDIR=/root/smsimulator/smsimulator5.5
        ENV PATH=/root/geant4_workdir/bin/Linux-g++:/root/geant4_v10/install/bin:/root/root_v5/install/bin:/root/cmake_v3.8/cmake_install/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/root/bin:/root/smsimulator/smsimulator5.5/bin/Linux-g++
        ENV G4SMLIBDIR=/root/smsimulator/smsimulator5.5/smg4lib
        ENV G4SMPHYSICSDIR=/root/smsimulator/smsimulator5.5/smg4lib/physics
        ENV G4SMACTIONDIR=/root/smsimulator/smsimulator5.5/smg4lib/action
        ENV G4SMCONSTRUCTIONDIR=/root/smsimulator/smsimulator5.5/smg4lib/construction
        ENV G4SMDATADIR=/root/smsimulator/smsimulator5.5/smg4lib/data
        ENV G4SMCPPFLAGS=-I/root/smsimulator/smsimulator5.5/smg4lib/include
        ENV G4SMLDLIBS='-lsmphysics -lsmaction -lsmconstruction -lsmdata'
        ENV LD_LIBRARY_PATH=/root/geant4_v10/install/lib64:/root/root_v5/install/lib:/root/anaroot_v4.5.39/anaroot/lib:/root/smsimulator/smsimulator5.5/smg4lib/lib:/root/smsimulator/smsimulator5.5/lib:/home/kondo/work/geant/anaroot/anaroot_v4.5.36/lib
        ENV LIBRARY_PATH=/root/smsimulator/smsimulator5.5/smg4lib/lib

        RUN echo "SMSIMDIR=/root/smsimulator/smsimulator5.5" >> /root/.bashrc
        RUN echo "PATH=/root/geant4_workdir/bin/Linux-g++:/root/geant4_v10/install/bin:/root/root_v5/install/bin:/root/cmake_v3.8/cmake_install/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/root/bin:/root/smsimulator/smsimulator5.5/bin/Linux-g++" >> /root/.bashrc
        RUN echo "G4SMLIBDIR=/root/smsimulator/smsimulator5.5/smg4lib" >> /root/.bashrc
        RUN echo "G4SMPHYSICSDIR=/root/smsimulator/smsimulator5.5/smg4lib/physics" >> /root/.bashrc
        RUN echo "G4SMACTIONDIR=/root/smsimulator/smsimulator5.5/smg4lib/action" >> /root/.bashrc
        RUN echo "G4SMCONSTRUCTIONDIR=/root/smsimulator/smsimulator5.5/smg4lib/construction" >> /root/.bashrc
        RUN echo "G4SMDATADIR=/root/smsimulator/smsimulator5.5/smg4lib/data" >> /root/.bashrc
        RUN echo "G4SMCPPFLAGS=-I/root/smsimulator/smsimulator5.5/smg4lib/include" >> /root/.bashrc
        RUN echo "G4SMLDLIBS='-lsmphysics -lsmaction -lsmconstruction -lsmdata'" >> /root/.bashrc
        RUN echo "LD_LIBRARY_PATH=/root/geant4_v10/install/lib64:/root/root_v5/install/lib:/root/anaroot_v4.5.39/anaroot/lib:/root/smsimulator/smsimulator5.5/smg4lib/lib:/root/smsimulator/smsimulator5.5/lib:/home/kondo/work/geant/anaroot/anaroot_v4.5.36/lib" >> /root/.bashrc
        RUN echo "LIBRARY_PATH=/root/smsimulator/smsimulator5.5/smg4lib/lib" >> /root/.bashrc

    WORKDIR /root/smsimulator/smsimulator5.5/smg4lib
    RUN make -j$(nproc)
    WORKDIR /root/smsimulator/smsimulator5.5/simdayone
    # DONT USE -j<something> for making simdayone!!! make craps out for some reason, about some .o file in ../tmp
    RUN make
    RUN cp -r /root/smsimulator/smsimulator5.5/work /root/smsimulator/
    WORKDIR /root/smsimulator/work

# # Install necessary packages and enable EPEL repository for htop
# #RUN yum update -y && yum install -y epel-release && yum clean all && yum install -y htop

# # Change root user pw and enable SSH access
#     RUN yum install -y openssh-server
#     RUN echo 'root:rootsucksballs' | chpasswd
#     RUN mkdir /var/run/sshd && /usr/bin/ssh-keygen -A
#     RUN sed -i 's/^#PermitRootLogin.*/PermitRootLogin yes/' /etc/ssh/sshd_config && sed -i 's/^#PasswordAuthentication.*/PasswordAuthentication yes/' /etc/ssh/sshd_config

# # Expose SSH port and start service
#     EXPOSE 22
#     CMD ["/usr/sbin/sshd", "-D"]
