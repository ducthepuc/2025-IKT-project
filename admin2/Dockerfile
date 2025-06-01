FROM ubuntu:22.04

RUN apt update && apt install -y \
    g++ \
    sqlite3 \
    libsqlite3-dev

WORKDIR /app

COPY . /app

RUN g++ ./server.cpp -o server -lsqlite3

EXPOSE 3000

CMD ["./server"]