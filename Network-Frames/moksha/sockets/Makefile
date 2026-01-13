# ================= client-server interaction =================
client_server_interaction:
	gcc client-Server_Interaction_s.c -o s
	gcc client-Server_Interaction_c.c -o c

# ================= iterative server =================
iterative:
	gcc IterativeServer-ultipleClients_S.c -o s
	gcc IterativeServer–MultipleClients_C.c -o c

# ================= just =================
just:
	gcc just_s.c -o s
	gcc just_c.c -o c

# ================= one way =================
one_way:
	gcc one_way_server.c -o s
	gcc one_way_client.c -o c

# ================= opcode =================
opcode:
	gcc Opcode_S.c -o s
	gcc Opcode_C.c -o c

# ================= plain text req-res =================
plain:
	gcc plain_text_res_req_S.c -o s
	gcc plain_text_res_req_C.c -o c

# ================= structured text protocol =================
structured:
	gcc Structured_Text_Protocol_S.c -o s
	gcc Structured_Text_Protocol_C.c -o c

# ================= two request-response =================
two_cycle:
	gcc two_request-response_cycle_server.c -o s
	gcc two_request-response_cycle_client.c -o c


# ================= two request-response =================
real:
	gcc Binary_Packet_Format_C.c -o s
	gcc Binary_Packet_Format_S.c -o c


# ================= clean =================
clean:
	rm -f *_s *_c 
