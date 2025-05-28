CC_RASP = /home/tiago/buildroot/output/host/bin/arm-buildroot-linux-gnueabihf-g++
CFLAGS = -Wall -std=c++17
LDLIBS = -lpthread -lrt -lsqlite3

# Lista de arquivos fonte
SOURCES = main.cpp Database.cpp msgqueue.cpp mqHandler.cpp 

# Diretório para armazenar os arquivos objeto
OBJDIR_RASP = obj_rasp

# Lista de arquivos objeto gerados a partir dos arquivos fonte
OBJECTS_RASP = $(addprefix $(OBJDIR_RASP)/, $(SOURCES:.cpp=.o))

# Nome do executável final
EXECUTABLE_RASP = daemonRasp

all: $(SOURCES) $(EXECUTABLE_RASP)

$(EXECUTABLE_RASP): $(OBJECTS_RASP)
	$(CC_RASP) $(OBJECTS_RASP) -o $@ $(LDLIBS)

$(OBJDIR_RASP)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CC_RASP) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR_RASP) $(EXECUTABLE_RASP)
