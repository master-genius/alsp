#ifndef VNODE_LIST_H
#define VNODE_LIST_H

#define VNODE_ADD_FAST      1223

#define FREE_DATA           1230

struct vnode_list {
    void * data;
    struct vnode_list *next;
    struct vnode_list *prev;
    struct vnode_list *last;
}vnode_list;

struct vnode_list *
vnode_list_init(struct vnode_list* vhnode);


void vnode_list_free(struct vnode_list * vhnode, int free_data);

struct vnode_list *
vnode_list_add(struct vnode_list * vhnode, 
        unsigned int data_size, void *idata, 
        void*(*addcall)(void *data, void *idata)
);


void
vnode_list_del(struct vnode_list * vhnode, struct vnode_list * pnode);

struct vnode_list *
vnode_list_find(struct vnode_list* start,
        void *need,
        int (*findcall)(void *data, void *need)
);

#endif

