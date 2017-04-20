#include <stdio.h>
#include "avltree.h"

static int arr[] = {3,2,1,4,5,6,7,16,15,14,13,12,11,10,8,9};
#define TBL_SIZE(a) ((sizeof(a)) / (sizeof(a[0])))

int main(void)
{
    int i, ilen;
    avltree root = NULL;

    printf("== 依次添加:");
    ilen = TBL_SIZE(arr);
    for (i = 0; i < ilen; i++)
    {
        printf("%d ", arr[i]);
        root = insert_node(root, arr[i]);
        printf("zhongxu:");
        inorder_avltree(root);
        printf("\n");
    }

    printf("\n== 前序遍历: ");
    preorder_avltree(root);
    printf("\n== 中序遍历: ");
    inorder_avltree(root);
    printf("\n== 后序遍历: ");
    postorder_avltree(root);
    printf("\n");

    printf("== 高度: %d\n", height(root));
    /*printf("==最大值: %d\n", findmax(root)->element);*/
    /*printf("==最小值: %d\n", findmin(root)->element);*/

    i = 8;
    printf("\n== 删除节点: %d", i);
    root = delete_node(root, i);

    printf("\n== 高度: %d\n", height(root));
    printf("\n== 中序遍历: ");
    inorder_avltree(root);

    destroy_avltree(root);
}
