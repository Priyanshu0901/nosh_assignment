FROM ubuntu:latest

RUN mkdir -p /app

WORKDIR /app

RUN apt update && apt install -y gcc

COPY assignment_2.c /app

RUN gcc assignment_2.c -o a_2

CMD ["/app/a_2"]