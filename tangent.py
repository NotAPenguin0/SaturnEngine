import argparse
import os.path
import os
import math

class PathError(Exception):
    pass

class FileNotFound(Exception):
    pass

class AttributeNotFound(Exception):
    pass

class InvalidAttribute(Exception):
    pass

class Vec3:
    def __init__(self, x = 0, y = 0, z = 0):
        self.x = x
        self.y = y
        self.z = z

    def __str__(self):
        return f"{self.x}, {self.y}, {self.z},"
        
    def __repr__(self):
        return str(self)

    def __sub__(self, other):
        return Vec3(self.x - other.x, self.y - other.y, self.z - other.z)

    def __mul__(self, scalar: float):
        return Vec3(self.x * scalar, self.y * scalar, self.z * scalar)

    def as_arr(self):
        return [self.x, self.y, self.z]

    def magnitude(self):
        return math.sqrt(self.x ** 2 + self.y ** 2 + self.z ** 2)

    def normalized(self):
        return self * (1 / self.magnitude())

class Vec2:
    def __init__(self, x = 0, y = 0):
        self.x = x
        self.y = y

    def __str__(self):
        return f"{self.x}, {self.y},"
        
    def __repr__(self):
        return str(self)

    def __sub__(self, other):
        return Vec2(self.x - other.x, self.y - other.y)


    def as_arr(self):
        return [self.x, self.y]
    
def make_vec3(data):
    return Vec3(data[0], data[1], data[2])
def make_vec2(data):
    return Vec2(data[0], data[1])


class MeshAttribute:
    def __init__(self, name = "", location = 0, size = 0):
        self.name = name
        self.location = int(location)
        self.size = int(size)

    def __str__(self):
        return f"<{self.name}, {self.location}, {self.size}>"

    def __repr__(self):
        return str(self)

class Vertex:
    def __init__(self, data = [], attributes = []):
        self.data = data
        self.attributes = attributes

    def __str__(self):
        return f"{self.data}"

    def __repr__(self):
        return str(self)

    def get_attribute(self, attribute: str):
        total_size = 0
        for attr in self.attributes:
            if attr.name == attribute:
                if attr.size == 2:
                    return make_vec2(
                        self.data[total_size:total_size + attr.size])
                elif attr.size == 3:
                    return make_vec3(
                        self.data[total_size:total_size + attr.size])
                    
            else:
                total_size += attr.size
        raise AttributeNotFound(f"Invalid attribute {attribute}")

    def add_attribute(self, attribute: MeshAttribute, data):
        if len(data.as_arr()) != attribute.size:
            raise InvalidAttribute("Attrubute size does not match data size")
        self.data += data.as_arr()
        self.attributes.append(attribute)

class Mesh:
    def __init__(self, vertices = [], attributes = []):
        self.vertices = vertices
        self.attributes = attributes
    def add_attribute(self, attribute: MeshAttribute):
        self.attributes.append(attribute)
    
        

def get_mesh_path():
    parser = argparse.ArgumentParser(description="Calculate tangents of a mesh")
    parser.add_argument("path", metavar = 'P', type=str, help="The path to the mesh to process")
    args = parser.parse_args()
    return args.path
    
def validate_path(path: str):
    mesh_ext = ".mesh"
    if not os.path.exists(path):
        raise PathError("Path does not exist")
    if not os.path.isfile(path):
        raise FileNotFound("Path is not a file")
    if not path.endswith(mesh_ext):
        raise TypeError("Path is not a mesh")

def parse_attributes(file):
    first_line = file.readline().strip()
    # The first entry in the file is the "attributes:" string,
    # after that comes a number
    attr_count = int(first_line.split(' ')[1])
    attributes = []
    for i in range(attr_count):
        line = file.readline().strip()
        data = line.split(' ')
        attributes.append(MeshAttribute(data[0], data[1], data[2]))

    return attributes

def parse_vertices(file, attributes):
    vertices = []
    # parse vertex count
    first_line = file.readline().strip()
    vtx_count = int(first_line.split(' ')[1])
    for i in range(vtx_count):
        vtx = Vertex([], [])
        line = file.readline().strip()
        line += ' '
        data = [float(x) for x in line.split(", ")[:-1]]
        cur = 0
        for attr in attributes:
            if attr.size == 2:
                x = data[cur]
                y = data[cur + 1]
                vtx.add_attribute(attr, Vec2(x, y))
            elif attr.size == 3:
                x = data[cur]
                y = data[cur + 1]
                z = data[cur + 2]
                vtx.add_attribute(attr, Vec3(x, y, z))
            cur += attr.size
        vertices.append(vtx)
    return vertices


def calculate_tangents(mesh: Mesh):
    mesh.add_attribute(MeshAttribute("Tangent", 3, 3))
    for i in range(int(len(mesh.vertices) / 3)):
        i1 = i * 3
        i2 = (i * 3 + 1) % len(mesh.vertices)
        i3 = (i * 3 + 2) % len(mesh.vertices)
        v1 = mesh.vertices[i1]
        v2 = mesh.vertices[i2]
        v3 = mesh.vertices[i3]
        e1 = v2.get_attribute("Position") - v1.get_attribute("Position")
        e2 = v3.get_attribute("Position") - v1.get_attribute("Position")

        duv1 = v2.get_attribute("TexCoords") - v1.get_attribute("TexCoords")
        duv2 = v3.get_attribute("TexCoords") - v1.get_attribute("TexCoords")
        f = 1 / (duv1.x * duv2.y - duv2.x * duv1.y)
        t = Vec3(0, 0, 0)
        t.x = f * (duv2.y * e1.x - duv1.y * e2.x)
        t.y = f * (duv2.y * e1.y - duv1.y * e2.y)
        t.z = f * (duv2.y * e1.z - duv1.y * e2.z)

        t = t.normalized()
        v1.add_attribute(MeshAttribute("Tangent", 3, 3), t)
        v2.add_attribute(MeshAttribute("Tangent", 3, 3), t)
        v3.add_attribute(MeshAttribute("Tangent", 3, 3), t)

def save_mesh(mesh, path):
    file = open(path, "w")
    file.write("attributes: %d\n" % len(mesh.attributes))
    for attr in mesh.attributes:
        file.write("%s %d %d\n" % (attr.name, attr.location, attr.size))
    file.write("vertices: %d\n" % len(mesh.vertices))
    for vtx in mesh.vertices:
        vtx_str = str(vtx)
        # remove [] and add a comma at the end
        vtx_str = vtx_str[1:-1]
        vtx_str += ','
        file.write(vtx_str + "\n")
    file.write("indices: 0\n")
    file.close()
    print("Generated tangents for mesh", os.path.basename(path))
    print("Saved to ", path)
        
def main():
    path = get_mesh_path()
    cwd = os.getcwd()
    path = os.path.join(cwd, path)
    validate_path(path)
    file = open(path) 
    attributes = parse_attributes(file)
    vertices = parse_vertices(file, attributes)
    file.close()
    mesh = Mesh(vertices, attributes)
    calculate_tangents(mesh)
    save_mesh(mesh, path)

if __name__ == "__main__":
    main()
