#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vlist.h"

struct vnode_list *
vnode_list_init(struct vnode_list* vhnode) {
    vhnode->next = NULL;
    vhnode->prev = NULL;
    vhnode->last = vhnode;
    vhnode->data = NULL;

    return vhnode;
}

void vnode_list_free(struct vnode_list * vhnode, int free_data) {

     struct vnode_list * ptmp;
     struct vnode_list * start;
     start = vhnode->next;
     ptmp = start;

     while(start!=NULL) {
         ptmp = start->next;
         if (free_data == FREE_DATA)
            free(start->data);
         free(start);
         start = ptmp;
     }
     vnode_list_init(vhnode);
}

struct vnode_list *
vnode_list_add(struct vnode_list * vhnode, 
        unsigned int data_size, void *idata,
        void*(*addcall)(void *data, void *idata)
) {
    struct vnode_list * ptmp;

    unsigned int vnode_size = sizeof(struct vnode_list);
    
    ptmp = (struct vnode_list*)malloc(vnode_size);
    if (ptmp == NULL) {
        perror("malloc");
        return NULL;
    }
    if (data_size <= 0)
        ptmp->data = idata;
    else {
        ptmp->data = (void*)malloc(data_size);
        if (ptmp->data == NULL) {
            free(ptmp);
            perror("malloc");
            return NULL;
        }
    }


    vhnode->last->next = ptmp;
    ptmp->next = NULL;
    ptmp->prev = vhnode->last;
    vhnode->last = ptmp;

    if (addcall != NULL) {
        addcall(ptmp->data, idata);
    } else {
        if (data_size > 0)
            memcpy(ptmp->data, idata, data_size);
    }

    return ptmp;
}


void
vnode_list_del(struct vnode_list * vhnode, struct vnode_list * pnode) {
    //printf("%p\n", pnode);
    if (pnode == NULL)return ;

    //如果是首节点则不释放
    if (pnode->prev == NULL)return ;

    pnode->prev->next = pnode->next;
    
    if (pnode->next)
        pnode->next->prev = pnode->prev;

    if (vhnode->last == pnode) {
        vhnode->last = pnode->prev;
    }
    //printf("%p\n", pnode);
    free(pnode);
}

struct vnode_list *
vnode_list_find(struct vnode_list* start,
        void *need,
        int (*findcall)(void *data, void *need)
) {
    struct vnode_list *nd;
    nd = start;

    if (findcall == NULL)
        return NULL;

    while(nd) {
        if (findcall(nd->data, need) == 1) {
            return nd;
        }
        nd = nd->next;
    }

    return NULL;
}

