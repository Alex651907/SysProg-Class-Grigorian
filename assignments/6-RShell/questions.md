1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?

The client uses a special character (i.e. 0x04) to mark the end of the message. It reads data in chunks with recv() and checks the last byte to see if it has the delimiter. Some other techniques include sending the length along with the message so the client knows how long the message should be.

2. This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?

The shell can use a delimiter such as \0 or length prefixes to define message boundaries. If messages are not handled correctly, it could result in fragmentation, splitting messages across multiple recv() calls. It coulud also result in stitching, combining multiple messages incorrectly. It could also just result in an infinite loop and lock the user out from doing any more until it finishes.

3. Describe the general differences between stateful and stateless protocols.

Stateful protocols maintain their state between requests, whereas stateless protocols do not. Some stateful protocols include FTP and SSH, and stateless protocols include HTTP and DNS. Stateful requires a persistent connection, whereas stateless just requires individual connection per request.

4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?

UDP is faster and simpler than TCP, which makes it ideal for real-time services like online video games or streaming. It's best for less important stuff where packet loss isn't a big issue, such as live video on a Zoom call or something.

5. What interface/abstraction is provided by the operating system to enable applications to use network communications?

The socket API is used for network communications. It can create a socket, assign an address to the socket, wait for connections, connect to a server, send and receive data, and end the connection.