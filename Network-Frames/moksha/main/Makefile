# ================= client-server interaction =================
normal:
	gcc 02_server_engine.c aes_128.c trng.c -o server
	gcc 01_client_trans.c aes_128.c trng.c -o client

# ================= iterative server =================
function:
	gcc 03_engine_ecu.c aes_128.c trng.c -o server
	gcc 01_client_trans.c aes_128.c trng.c -o client
# ================= clean =================
clean:
	rm -rf server client

