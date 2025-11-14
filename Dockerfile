FROM alpine:lastest
RUN apk add --no-cache gcc musl-dev make
COPY . /backend_elecam
WORKDIR /backend_elecam
 RUN make
 EXPOSE 8080
 CMD ["./server"]
