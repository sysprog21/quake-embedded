add_executable(quake
	main.c
	display.c
	fio.c
)

target_link_libraries(quake
	winquake
	port
	m
)
