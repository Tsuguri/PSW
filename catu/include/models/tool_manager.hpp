#pragma once

#include <QObject>
#include <QQmlListProperty>

#include <tools/mill.hpp>

class ToolManager : public QObject {
    Q_OBJECT

    Q_PROPERTY(QQmlListProperty<Mill> tools READ getTools NOTIFY toolsChanged);

    public:
        explicit ToolManager();

        Mill* getTool(int radius, MillType type);

        QQmlListProperty<Mill> getTools();

        Q_INVOKABLE void AddTool(int radius, short type);
        Q_INVOKABLE void RemoveTool(int index);

        void appendTool(Mill* mill);
        int toolsCount() const;
        Mill* tool(int id) const;
        void clearTools();

signals:
        void toolsChanged();
    private:
        static void appendTool(QQmlListProperty<Mill>* prop, Mill* mill);
        static int toolsCount(QQmlListProperty<Mill>* prop);
        static Mill* tool(QQmlListProperty<Mill>* prop, int index);
        static void clearTools(QQmlListProperty<Mill>* prop);
        std::vector<Mill*> _mills;
};
