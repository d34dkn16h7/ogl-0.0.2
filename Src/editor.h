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
    int sIndex = 0;
    bool isMultyEdit;
    static vector<GameObject*> selection;
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
    void Update();
    static bool isSelected(GameObject*);
    GameObject* GetSelection(unsigned int);
};

#endif // EDITOR_H
