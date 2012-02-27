# ##### BEGIN GPL LICENSE BLOCK #####
#
#  This program is free software; you can redistribute it and/or
#  modify it under the terms of the GNU General Public License
#  as published by the Free Software Foundation; either version 2
#  of the License, or (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software Foundation,
#  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
#
# ##### END GPL LICENSE BLOCK #####

# <pep8-80 compliant>

bl_info = {
    "name": "PyUniverse OBJ format",
    "author": "PyUniverse folks, based on code from Campbell Barton",
    "blender": (2, 5, 8),
    "api": 35622,
    "location": "File > Import-Export",
    "description": ("Export OBJ"),
    "warning": "",
    "tracker_url": "",
    "support": "COMMUNITY",
    "category": "Import-Export"}

if "bpy" in locals():
    import imp
    if "export_pyuni_obj" in locals():
        imp.reload(export_pyuni_obj)


import bpy
from bpy.props import (BoolProperty,
                       FloatProperty,
                       StringProperty,
                       EnumProperty,
                       )
from bpy_extras.io_utils import (ExportHelper,
                                 ImportHelper,
                                 path_reference_mode,
                                 axis_conversion,
                                 )


class ExportOBJ(bpy.types.Operator, ExportHelper):
    '''Save a PyUniverse OBJ File'''

    bl_idname = "export_scene.obj"
    bl_label = 'Export OBJ'
    bl_options = {'PRESET'}

    filename_ext = ".obj"
    filter_glob = StringProperty(
            default="*.obj;*.mtl",
            options={'HIDDEN'},
            )

    # context group
    use_selection = BoolProperty(
            name="Selection Only",
            description="Export selected objects only",
            default=False,
            )
    use_all_scenes = BoolProperty(
            name="All Scenes",
            description="",
            default=False,
            )
    use_animation = BoolProperty(
            name="Animation",
            description="",
            default=False,
            )

    # object group
    use_apply_modifiers = BoolProperty(
            name="Apply Modifiers",
            description="Apply modifiers (preview resolution)",
            default=True,
            )

    # extra data group
    use_edges = BoolProperty(
            name="Edges",
            description="",
            default=True,
            )
    use_normals = BoolProperty(
            name="Normals",
            description="",
            default=True,
            )
    use_hq_normals = BoolProperty(
            name="High Quality Normals",
            description="",
            default=True,
            )
    use_uvs = BoolProperty(
            name="UVs",
            description="",
            default=True,
            )
    use_materials = BoolProperty(
            name="Materials",
            description="",
            default=True,
            )
    use_triangles = BoolProperty(
            name="Triangulate",
            description="",
            default=True,
            )
    use_vertex_groups = BoolProperty(
            name="Polygroups",
            description="",
            default=False,
            )
    use_nurbs = BoolProperty(
            name="Nurbs",
            description="",
            default=False,
            )

    # grouping group
    use_blen_objects = BoolProperty(
            name="Objects as OBJ Objects",
            description="",
            default=True,
            )
    group_by_object = BoolProperty(
            name="Objects as OBJ Groups ",
            description="",
            default=False,
            )
    group_by_material = BoolProperty(
            name="Material Groups",
            description="",
            default=False,
            )
    keep_vertex_order = BoolProperty(
            name="Keep Vertex Order",
            description="",
            default=False,
            )

    global_scale = FloatProperty(
            name="Scale",
            description="Scale all data",
            min=0.01, max=1000.0,
            soft_min=0.01,
            soft_max=1000.0,
            default=1.0,
            )

    axis_forward = EnumProperty(
            name="Forward",
            items=(('X', "X Forward", ""),
                   ('Y', "Y Forward", ""),
                   ('Z', "Z Forward", ""),
                   ('-X', "-X Forward", ""),
                   ('-Y', "-Y Forward", ""),
                   ('-Z', "-Z Forward", ""),
                   ),
            default='-Z',
            )

    axis_up = EnumProperty(
            name="Up",
            items=(('X', "X Up", ""),
                   ('Y', "Y Up", ""),
                   ('Z', "Z Up", ""),
                   ('-X', "-X Up", ""),
                   ('-Y', "-Y Up", ""),
                   ('-Z', "-Z Up", ""),
                   ),
            default='Y',
            )

    path_mode = path_reference_mode

    check_extension = True

    def execute(self, context):
        from . import export_pyuni_obj

        from mathutils import Matrix
        keywords = self.as_keywords(ignore=("axis_forward",
                                            "axis_up",
                                            "global_scale",
                                            "check_existing",
                                            "filter_glob",
                                            ))

        global_matrix = Matrix()

        global_matrix[0][0] = \
        global_matrix[1][1] = \
        global_matrix[2][2] = self.global_scale

        global_matrix = (global_matrix *
                         axis_conversion(to_forward=self.axis_forward,
                                         to_up=self.axis_up,
                                         ).to_4x4())

        keywords["global_matrix"] = global_matrix
        return export_pyuni_obj.save(self, context, **keywords)


def menu_func_export(self, context):
    self.layout.operator(ExportOBJ.bl_idname, text="PyUniverse (.obj)")


def register():
    bpy.utils.register_module(__name__)

    bpy.types.INFO_MT_file_export.append(menu_func_export)


def unregister():
    bpy.utils.unregister_module(__name__)

    bpy.types.INFO_MT_file_export.remove(menu_func_export)

if __name__ == "__main__":
    register()
