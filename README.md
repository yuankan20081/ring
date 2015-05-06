# ring
环形链表buffer
gnu g++ compiler version : 4.8.2;
任意时间点只允许一个线程进行Read，任意时间点只允许一个线程进行Write，但Read和Write可以同时进行而不需要再使用mutex;
