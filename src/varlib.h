#ifndef VARLIB_H
#define VARLIB_H

/**
 * 变量管理
 * 用于存储、查找、导出和列出变量
 */
namespace shellVarlib
{
    /** 将一个变量存储在 tab 中。如果变量已存在，则更新其值；如果变量不存在，则创建新变量 */
    int VLstore(char *name, char *val);
    /** 查找变量 name 的值。如果找到，返回其值；否则，返回空字符串 */
    char *VLlookup(char *name);
    /** 将变量 name 标记为全局变量。如果变量不存在，则首先存储该变量，然后再标记为全局变量 */
    int VLexport(char *name);
    /**  列出所有变量及其值，并标记出全局变量 */
    void VLlist();
    /** 将环境变量数组转换为 tab 数组 */
    int VLenviron2table(char *env[]);
    /** 将 tab 数组转换为环境变量数组 */
    char **VLtable2environ();
}

#endif // VARLIB_H
