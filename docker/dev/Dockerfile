FROM archlinux:latest

RUN pacman -Syu --noconfirm && \
	pacman -S --noconfirm base base-devel clang git llvm18 cmake ninja lld18

RUN useradd -m user && echo "user ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers
USER user 
WORKDIR /home/user

RUN git clone https://aur.archlinux.org/cmake-format.git
RUN cd cmake-format && makepkg -si --noconfirm
RUN rm -rf cmake-format

USER root

RUN pacman -Scc --noconfirm

ENV CC=clang
ENV CXX=clang++
ENV LLVM_DIR=/usr/lib/llvm18/lib/cmake/llvm
ENV LLD_DIR=/usr/lib/llvm18/lib/cmake/lld

WORKDIR /lily
VOLUME /lily
USER user

ENTRYPOINT ["/bin/bash"]
