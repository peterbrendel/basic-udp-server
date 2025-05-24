FROM alpine:latest AS builder

# Install build dependencies
RUN apk add --no-cache \
    alpine-sdk \
    cmake \
    git \
    libstdc++

WORKDIR /app

COPY . .

RUN chmod +x ./compile && ./compile clean release

##             ##
# Runtime image #
##             ##

FROM alpine:latest

# Install runtime dependencies
RUN apk add --no-cache \
    libstdc++

WORKDIR /app

COPY --from=builder /app/build/Release/sparz ./sparz

ENTRYPOINT ["./sparz", "-w", "4"]
