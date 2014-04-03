#ifndef EDITOR_H
#define EDITOR_H

#include <glm/glm.hpp>
#include <vector>

/// This should be static

using namespace std;
using namespace glm;

enum EditMode
{PositionEdit,ScaleEdit,RotationEdit,ColorEdit};

class GameObject;

class Editor
{
private:
    static vector<GameObject*> selection;

    static EditMode mode;
    static bool isMultyEdit;

    static void Edit();
    static void PutObject();
    static void DeleteObject();
    static void SelectObjects();
    static void UpdateSelection(GameObject*);
    static void UpdateSelections(const vector<GameObject*> );
    static void RemoveSelection(GameObject*);
    static void ClearSelection();

    static void aaScale(vec3); /// Add scale to all selected objects
    static void aaPosition(vec3); /// Move all selected objects

    static void MoveCam();
public:
    static void Update();
    static bool isSelected(GameObject*);
    static GameObject* GetSelection(unsigned int);
};

#endif // EDITOR_H
