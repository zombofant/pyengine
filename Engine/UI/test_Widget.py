from Widget import Widget
import unittest

class WidgetInit(unittest.TestCase):
    def test_init(self):
        instance = Widget()
        self.assertEqual(len(instance), 0)
        self.assertIsNone(instance.Parent)
        del instance

    def test_separate(self):
        instanceA = Widget()
        instanceB = Widget()
        self.assertIsNot(instanceA._children, instanceB._children)

class WidgetInstanceTest(unittest.TestCase):
    def setUp(self):
        self.instance = Widget()

    def tearDown(self):
        del self.instance

class WidgetChildren(WidgetInstanceTest):
    def test_add(self):
        child = Widget()
        self.instance.add(child)
        self.assertEqual(len(self.instance), 1)
        self.assertIs(child.Parent, self.instance)
        self.assertIn(child, self.instance)
        self.assertIs(self.instance[0], child)
    
    def test_dupe(self):
        child = Widget()
        self.instance.add(child)
        self.assertRaises(ValueError, self.instance.add, child)

    def test_multipleParents(self):
        parentB = Widget()
        child = Widget()
        self.instance.add(child)
        self.assertRaises(ValueError, parentB.add, child)

    def test_multipleChildren(self):
        childA, childB, childC = Widget(), Widget(), Widget()
        self.instance.add(childA)
        self.instance.add(childB)
        self.instance.add(childC)
        self.assertEqual(list(self.instance), [childA, childB, childC])
        self.instance.bringToFront(0)
        self.assertEqual(list(self.instance), [childB, childC, childA])
        del self.instance[0]
        self.assertEqual(list(self.instance), [childC, childA])

    def test_typeEnforcement(self):
        self.assertRaises(TypeError, self.instance.add, 1)

    def test_init(self):
        child = Widget(parent=self.instance)
        self.assertEqual(len(self.instance), 1)
        self.assertEqual(self.instance[0], child)
