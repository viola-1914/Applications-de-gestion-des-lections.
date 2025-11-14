FROM gcc:13.2

COPY . /backend_elecam
WORKDIR /backend_elecam
 RUN Makefile
 EXPOSE 8080
 CMD ["./server"]
