; Initialize stack and B-tree setup
ldc 0x1000            ; Load starting address of the stack
a2sp                  ; Initialize stack pointer
ldc 0                 ; Load 0 (root node initially)
stl 0                 ; Store root node pointer

; Initialize dynamic memory start at address 0
ldc 0                 ; Starting memory address
stl mem_start         ; Store in `mem_start`

; Allocate memory for the root node
ldc root_node_size    ; Size of a node in words
call alloc_mem        ; Call memory allocation subroutine
stl 0                 ; Store root node pointer
ldl 0                 ; Load root node pointer
stnl 0                ; Initialize root node key count to 0

; Insertion Entry Point
ldl 0                 ; Load the address of the root node
ldc 15                ; Load key to insert
call btree_insert     ; Call the B-Tree insert function
HALT                  ; End of program

; B-Tree Insert Subroutine
btree_insert:
    adj -3            ; Allocate space for local variables
    stl 2             ; Store key to insert
    stl 1             ; Store current node pointer

    ; Check if current node is full
    ldl 1             ; Load current node pointer
    ldnl 0            ; Load the key count of the node
    ldc 3
    sub               ; Compare key count with 3 (max keys in a node)
    brlz insert_key   ; If key count < 3, insert directly

    ; Node is full, handle split (simplified single level split)
    ldl 1             ; Load current node pointer
    call split_node   ; Split the current node
    stl 3             ; Save new node pointer after split

    ; After split, decide where to insert the key (simplified)
    ldl 3             ; Load new node pointer
    ldl 2             ; Load key to insert
    call insert_key_in_node ; Insert key in appropriate new node
    br end_insert

insert_key:
    ldl 1             ; Load current node pointer
    ldl 2             ; Load key to insert
    call insert_key_in_node ; Insert key in node

end_insert:
    adj 3             ; Deallocate local variables
    return            ; Return to caller

; Insert Key in Node Subroutine
insert_key_in_node:
    adj -3            ; Allocate space for local variables
    stl 2             ; Store key
    stl 1             ; Store node pointer

    ; Find position to insert key
    ldl 1             ; Load node pointer
    ldnl 0            ; Load current key count in the node
    stl 3             ; Store key count temporarily

    ; Insert key in sorted position (simplified linear search)
    ldl 1             ; Load node pointer
    ldl 3             ; Load key count
    ldc 1
    add               ; Increment key count
    stnl 0            ; Update key count in node

    ; Append the key to the next available position
    ldl 1             ; Load node pointer
    ldc 1             ; Offset for key storage
    ldl 3             ; Load current key count
    add               ; Compute address for new key
    ldl 2             ; Load key
    stnl 0            ; Store key
    adj 3             ; Deallocate local variables
    return            ; Return from insertion

; Split Node Subroutine
split_node:
    adj -2            ; Allocate space for local variables
    stl 1             ; Store node pointer

    ; Allocate memory for a new node
    ldc root_node_size
    call alloc_mem
    stl 2             ; Save new node address

    ; Move middle key to new node
    ldl 1             ; Load current node pointer
    ldc 2             ; Offset for middle key
    ldnl 2            ; Load middle key
    ldl 2             ; Load new node pointer
    ldc 1
    stnl 0            ; Move middle key to new node
    ldc 1
    stnl 0            ; Set key count of new node to 1

    ; Update current node key count
    ldl 1             ; Load current node pointer
    ldc 1
    stnl 0            ; Set key count to 1
    ldl 2             ; Return the new node pointer
    adj 2             ; Deallocate local variables
    return

; Memory Allocation Subroutine
alloc_mem:
    adj -1            ; Allocate space for local variables
    ldl mem_start     ; Load current memory start address
    stl 0             ; Store allocated address (return value)
    ldl mem_start     ; Load memory start again
    ldl 1             ; Load size to allocate
    add               ; Compute new memory start
    stl mem_start     ; Update memory start
    adj 1             ; Deallocate local variables
    return

; Constants
root_node_size: data 8 ; Node size in words (key count + 3 keys + 4 pointers)

; Variables
mem_start: data 0      ; Starting address for dynamic memory allocation
