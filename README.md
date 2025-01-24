<h1 align="center">RCOM - Computer Networks</h1>

This project focused on the configuration, usage and analysis of computer networks. It was divided into two parts.

The first part consisted in the development of a simple FTP application, able to download a select file from a FTP server. The application was developed in C and used the FTP protocol to communicate with the server.

The second part focused on the core of this project - to setup a computer network in multiple configuration and analyze their effects on the connectivity and performance of the network. This part took place on a computer lab specifically prepared for this project, and was divided into a total of 6 experiments, to be developed throughout the practical classes. In these experiments, we had to physically connect the computers (by configuring Ethernet cables, switches and routers), adjust some networking parameters on Linux machines and analyze the network traffic using multiple tools, such as `ping`, `traceroute` and Wireshark. The results of each experiment (logs) were then analyzed and discussed in the report.

### Run Instructions

To run the project, run the following commands in the Linux terminal:

```sh
make                  # Compile the program (saved in bin/)
cd bin                
./download <ftp-url>  # Download a file matching the FTP URL provided
```

---

> Class: 3LEIC01 Group: 7  
> Final Grade: 20.0  
> Professors: Hélder Fontes  
> Created in November 2024 for RCOM (Redes de Computadores)  