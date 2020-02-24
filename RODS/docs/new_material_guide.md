# RODS 添加新材料

* RODS由C++语言采用面向对象的范式开发。

* 新材料以新C++类的形式加入。

* 新加一维材料应继承自Material1D类。
  * Material1D 已有成员变量：
    ``` C++
    int id;             ///< 材料编号
    double E;           ///< 切线模量
    double sigma; 	    ///< 应力
    double epsilon;     ///< 应变
    ```
  * 新材料类应重新定义Material1D的虚函数：
    ``` C++
    /// 设置材料的应变
    void setStrain(const double *strain);

    /// 根据应变计算应力、切线模量并更新历史变量
    /// 参数 update 用于控制是否更新历史变量
    void getResponse(const bool update = false);

    /// 复制一个新材料实例，并返回指针
    Material1D *copy();
    ```
  * getResponse 函数需要完成的任务：
    * 计算出 sigma, E 的数值
    * 更新历史变量

* 新类完成定义后，应在 DynamicSystem 类中增加新的成员函数。
    ```C++
    void addMaterialNewMaterial(const int id, ...) ///< NewMaterial 为新材料名
    {
        if ( checkDuplicateMaterial1D(id) )
        {
            Material1D *mt = new NewMaterial(id, ...);
            Material1Ds[id] = mt;
        }
    }
    ```