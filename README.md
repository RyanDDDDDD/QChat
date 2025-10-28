# 💬 Distributed Real-time Fullstack Chat Application (C++)

A comprehensive **C++ fullstack chat application** that demonstrates real-world distributed system design.  
The project includes a **Qt-based desktop client**, **asynchronous C++ servers using Boost.Asio**, an **HTTP gateway built with Boost.Beast**, a **Node.js authentication service**, and **gRPC communication** between distributed backend services.  
It provides a complete example of applying modern C++ in a full production-like environment, from frontend GUI to distributed backend architecture.

---

## 🏗️ Architecture Overview

<img width="1178" height="786" alt="image" src="https://github.com/user-attachments/assets/20885145-361f-44ce-af21-f0d1ec85ca15" />

- **GateServer (Gateway Service):**  
  Handles client connections and registration requests. Since the backend is distributed, the GateServer queries the **StatusServer** to select a backend server (e.g., ServerA or ServerB) with the lowest load and provides its address to the client.  
  The client then establishes a **persistent TCP connection** directly with the assigned chat server.

- **Verification Server (Node.js Service):**  
  Validates registration requests and sends verification codes to clients during account creation. The GateServer communicates with this service to ensure secure and valid user verification.

- **StatusServer:**  
  Monitors the status of all chat servers and manages load distribution. It allows GateServer to make routing decisions dynamically.

- **ServerA / ServerB (Chat Servers):**  
  Maintain long-lived TCP connections with clients and handle message delivery. Each server can access **Redis** and **MySQL** directly for data persistence and caching.  
  Messages are forwarded asynchronously through an internal message queue, enabling high concurrency and low latency.

---

## ⚙️ Tech Stack

- **Language & Frameworks:** C++17, Qt, Boost.Asio, Boost.Beast  
- **Concurrency:** Multithreading, Thread Pool, Asynchronous I/O  
- **Communication:** gRPC (inter-service), HTTP/TCP (client-server)  
- **Database & Cache:** MySQL, Redis  
- **Authentication:** Node.js (OAuth-style verification service)  
- **Architecture:** Distributed microservice-style design  
- **Design Patterns:** Singleton, Factory, Observer, and Reactor Pattern  

---

## ✨ Key Features

- 🖥 **Qt Frontend:**  
  Developed a cross-platform desktop chat client using **Qt Network module** to encapsulate HTTP and TCP communication.  

- ⚡ **Asynchronous Server:**  
  Implemented a high-performance chat server using **Boost.Asio**, achieving efficient non-blocking I/O operations.  

- 🔗 **Gateway Routing:**  
  The **Beast-based HTTP Gateway** handles client registration, routing, and authentication requests.  

- 🧱 **Distributed Design:**  
  Multiple backend servers (ServerA, ServerB) communicate via **gRPC**, sharing user data and message delivery tasks.  

- 🔒 **User Authentication:**  
  Integrated a **Node.js verification service** for secure registration and login via email verification codes.  

- 📈 **Scalability & Performance:**  
  Each server instance supports up to **8,000 concurrent connections**; through multi-server deployment, the system supports **10,000–20,000 active users** concurrently.  

---

## 🧩 Project Structure

    DistributedChatApp/
    │
    ├── client_qt/ # Qt-based desktop client
    ├── gate_server/ # HTTP gateway (Beast + Asio)
    ├── chat_server/ # Main chat service (Asio + gRPC)
    ├── status_server/ # Load monitor and service registry
    ├── verify_server/ # Node.js verification service
    ├── common/ # Shared code (protobufs, config, utils)
    └── scripts/ # Deployment and configuration scripts

---

## 🧠 Key Learnings

- Designed and implemented a **distributed system architecture** using modern C++.  
- Mastered **Boost.Asio asynchronous programming** and **multithreaded server design**.  
- Applied **gRPC** for inter-service communication and **Redis/MySQL** for data persistence.  
- Gained hands-on experience in **network programming**, **load balancing**, and **system scalability**.

---

## 🚀 Future Improvements

- Implement WebSocket support for real-time browser clients.  
- Add monitoring dashboards for system metrics and user activity.  
- Explore Kubernetes deployment for containerized scaling.
