function(CreateSubfolders SubfolderName)
    source_group("Header Files\\${SubfolderName}" REGULAR_EXPRESSION ".*/${SubfolderName}/.*.h$")
    source_group("Source Files\\${SubfolderName}" REGULAR_EXPRESSION ".*/${SubfolderName}/.*.cpp$")
endfunction()