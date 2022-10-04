FROM devkitpro/devkitarm:latest

# install makerom
WORKDIR /tmp
RUN apt-get update && \
    apt-get -y install build-essential clang-format

RUN git clone https://github.com/3DSGuy/Project_CTR.git && \
    cd Project_CTR && \
    make && \
    cp makerom/bin/makerom /usr/local/bin

COPY vendor /app/vendor
WORKDIR /app/vendor/sf2dlib/libsf2d
RUN make install
WORKDIR /app/vendor/sfillib/libsfil
RUN make install

WORKDIR /app
COPY ./ /app/
