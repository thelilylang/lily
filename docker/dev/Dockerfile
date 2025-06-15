FROM archlinux:latest

RUN pacman -Syu --noconfirm && \
	pacman -S --noconfirm base base-devel clang git

RUN pacman -Scc --noconfirm

RUN useradd user

ENV CC=clang
ENV CXX=clang++

WORKDIR /lily
VOLUME /lily
USER user

ENTRYPOINT ["/bin/bash"]
