* RODS_model.py 的类与 RODS 中的类一一对应
* RODS_GUI.py 中对于 RODS_model.py 的类定义一个 add???Widget 类和一个 ???TableWidget 类
* RODS_GUI.py 的 MainWindow 类中对于 RODS_model.py 的类提供一个 add???Action 菜单项；定义一个 add??? 函数

```python
add???Action.triggered.connect(self.add???Widget.show)
self.add???Widget.addButton.clicked.connect(self.add???)


```