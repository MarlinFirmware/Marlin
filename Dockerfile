FROM ubuntu:14.04.3

RUN apt-get update && apt-get install -y make gcc-avr avr-libc

WORKDIR /usr/src/myapp


