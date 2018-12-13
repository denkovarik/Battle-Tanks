se in 16.04
FROM ubuntu:16.04

# apt-get and install tools
RUN apt-get update
RUN apt-get install -y make gcc g++ lcov freeglut3-dev git

# remove apt files after installs
RUN rm -rf /var/lib/apt/lists/*
