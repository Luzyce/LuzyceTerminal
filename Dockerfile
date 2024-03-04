# Use an official Ubuntu image as the base
FROM ubuntu:latest

# Set the working directory
WORKDIR /app

# Install dependencies
RUN apt update && \
    apt install -y \
    python3 \
    python3-pip \
    git \
    curl

RUN pip3 install platformio

COPY appbuild.sh /usr/local/bin/appbuild.sh
