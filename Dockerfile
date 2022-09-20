FROM debian:latest
RUN \
    apt-get update && apt-get upgrade -y && \
    apt-get install -y build-essential openssh-server git && \
    git clone https://github.com/veloek/cnake.git /app && \
    make -C /app && \
    useradd -M -N -g nogroup -s /app/cnake cnake && \
    passwd -d cnake && \
    sed -i 's/#PermitEmptyPasswords no/PermitEmptyPasswords yes/g' /etc/ssh/sshd_config && \
    service ssh start

EXPOSE 22

CMD ["/usr/sbin/sshd", "-D"]
