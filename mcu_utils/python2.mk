
HEADER_DIRS  += $(addprefix -a ,$(EXPORT_INC_PATH))
HEADER_FILES += $(addprefix -f ,$(EXPORT_INC_FILE))
PYTHON_HEADER_SCRIPT = ~/MOCD/AddressInterceptor/mcu-lib/parser/header2.py
PY_VERSION = --version=$(MCU_MAJOR_VERSION_LIB)-`git rev-parse --short HEAD`
NAME_PY_FILE = ${DEVICE}_$(MCU_LIB_NAME).py
INCLUDE_STRING_EX = -i='$(FIRST_RUNTIME_STRING)'

python: $(NAME_PY_FILE)

$(NAME_PY_FILE):
	python $(PYTHON_HEADER_SCRIPT) -c "$(LOCAL_CFLAGS)" $(INCLUDE_STRING_EX) $(HEADER_DIRS) $(HEADER_FILES) $(PY_VERSION) -o $(NAME_PY_FILE) -d 1