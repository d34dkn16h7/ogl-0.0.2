#ifndef EDITOR_H
#define EDITOR_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

using namespace std;
using namespace glm;

enum EditMode
{PositionEdit,ScaleEdit,RotationEdit,ColorEdit};

class Map;
class GameObject;
class Editor
{
private:
    int sIndex = 0;
    bool isMultyEdit;
    vector<GameObject*> selection;
    Map* targetMap;
    GameObject* onEdit;
    EditMode mode;

    void Edit();
    void PutObject();
    void DeleteObject();
    void SelectObjects();
    void UpdateSelection(GameObject*);
    void UpdateSelections(const vector<GameObject*> );
    void RemoveSelection(GameObject*);
    void ClearSelection();

    void aaScale(vec3); /// Add scale to all selected objects
    void aaPosition(vec3); /// Move all selected objects

    void MoveCam();
public:
    void SetTargetMap(Map*);
    void Update();
    bool isSelected(GameObject*);
    GameObject* GetSelection(unsigned int);
};

#endif // EDITOR_H
