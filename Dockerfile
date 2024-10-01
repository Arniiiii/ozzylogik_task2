FROM gentoo/stage3:amd64-openrc

RUN echo "Updating Gentoo index registry..."
RUN emerge-webrsync

RUN echo "Updating gpg stuff related to gentoo's binary packages..."
RUN getuto

RUN echo "Setting up package manager"
# Allow emerge newer versions of cmake. We need it because vcpkg wants it, though we don't use vcpkg here.
RUN echo "dev-build/cmake ~amd64" >> /etc/portage/package.accept_keywords/unmask
# # Get some packages as binaries and don't compile them.
RUN rm -rf /etc/portage/binrepos.conf/*
RUN echo -e "[binhost]\npriority = 9999\nsync-uri = https://gentoo.osuosl.org/releases/amd64/binpackages/23.0/x86-64/" >> /etc/portage/binrepos.conf/osuosl.conf



RUN echo "Installing dependencies..."
RUN FEATURES="parallel-fetch parallel-install" emerge --verbose --tree --verbose-conflicts --getbinpkg=y --jobs=24 dev-vcs/git dev-build/cmake zip dev-qt/qtbase

# RUN echo "Installing vcpkg..."
# RUN git clone https://github.com/Microsoft/vcpkg.git && \
# 		./vcpkg/bootstrap-vcpkg.sh -disableMetrics 
#
# COPY ./vcpkg.json ./vcpkg.json
#
# RUN echo "Installing dependencies using vcpkg"
# RUN ./vcpkg/vcpkg install

WORKDIR /backend

RUN echo "Copy sources of the project"
COPY . .
RUN ls -lah

RUN echo "Configuring project..."
RUN cmake -S ./all -B ./build --log-level DEBUG -DFETCHCONTENT_QUIET=OFF


RUN echo "Building project..."
RUN cmake --build ./build --parallel $(nproc) --verbose

RUN echo "installing in a dir"
RUN cmake --install ./build --prefix ./install_dir

RUN echo "showing install dir"
RUN ls -lah -R ./install_dir

RUN echo "really installing"
RUN cmake --install ./build


