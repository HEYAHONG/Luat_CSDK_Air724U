if(CONFIG_APPIMG_LOAD_FLASH)
add_appimg(${target} ${flash_ldscript} demo_timer.c)
else(CONFIG_APPIMG_LOAD_FLASH)
add_appimg(${target} ${file_ldscript} demo_timer.c)
endif()