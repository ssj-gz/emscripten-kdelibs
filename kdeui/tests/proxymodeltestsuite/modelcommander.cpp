/*
    Copyright (c) 2009 Stephen Kelly <steveire@gmail.com>

    This library is free software; you can redistribute it and/or modify it
    under the terms of the GNU Library General Public License as published by
    the Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    This library is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
    License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to the
    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301, USA.
*/

#include "modelcommander.h"
#include "dynamictreemodel.h"


ModelCommander::ModelCommander(DynamicTreeModel* model, QObject *parent)
: QObject(parent), m_counter(0), m_model(model)
{
  setDefaultCommands();
}

void ModelCommander::setCommands(QList<QPair<QString, ModelChangeCommandList> > commands)
{
  m_commands = commands;
}

void ModelCommander::clear()
{
  m_commands.clear();
  m_counter = 0;
}

void ModelCommander::executeUntil(const QString& stopBefore)
{
  while(hasNextCommand())
  {
    QPair<QString, ModelChangeCommandList> nextChangeCommand = nextCommand();
    if (nextChangeCommand.first == stopBefore)
    {
      return;
    }
    executeNextCommand();
  }
}

void ModelCommander::executeNextCommand()
{
  if (!hasNextCommand())
    return;
  QPair<QString, ModelChangeCommandList> nextChangeCommand = nextCommand();

  ++m_counter;

  foreach(ModelChangeCommand *command, nextChangeCommand.second)
  {
    command->doCommand();
  }
}

bool ModelCommander::hasNextCommand()
{
  return m_commands.size() > m_counter;
}

QPair<QString, ModelChangeCommandList> ModelCommander::nextCommand()
{
  return m_commands.at(m_counter);
}

QStringList ModelCommander::commandNames() const
{
  QStringList list;

  for (int i = 0; i < m_commands.size(); ++i)
  {
    list << m_commands.at(i).first;
  }

  return list;
}

void ModelCommander::setDefaultCommands()
{
  m_counter = 0;

  // Insert a single item at the top.
  ModelInsertCommand *ins;
  ins = new ModelInsertCommand(m_model, this);
  ins->setStartRow(0);
  ins->setEndRow(0);

  ModelChangeCommandList commandList;

  commandList << ins;

  setCommand("insert01", commandList);
  commandList.clear();

  // Give the top level item 10 children.
  ins = new ModelInsertCommand(m_model, this);
  ins->setAncestorRowNumbers(QList<int>() << 0 );
  ins->setStartRow(0);
  ins->setEndRow(9);

  commandList << ins;
  setCommand("insert02", commandList);
  commandList.clear();

  // Give the top level item 10 'older' siblings.
  ins = new ModelInsertCommand(m_model, this);
  ins->setStartRow(0);
  ins->setEndRow(9);

  commandList << ins;
  setCommand("insert03", commandList);
  commandList.clear();

  ModelMoveLayoutChangeCommand *moveCommand = new ModelMoveLayoutChangeCommand(m_model, this);
  moveCommand->setAncestorRowNumbers(QList<int>() << 10);
  moveCommand->setStartRow(0);
  moveCommand->setEndRow(0);
  moveCommand->setDestAncestors(QList<int>() << 10);
  moveCommand->setDestRow(5);

  commandList << moveCommand;

  setCommand("move01", commandList);
  commandList.clear();

  moveCommand = new ModelMoveLayoutChangeCommand(m_model, this);
  moveCommand->setAncestorRowNumbers(QList<int>() << 10);
  moveCommand->setStartRow(4);
  moveCommand->setEndRow(4);
  moveCommand->setDestAncestors(QList<int>() << 10);
  moveCommand->setDestRow(0);

  commandList << moveCommand;

  setCommand("move02", commandList);
  commandList.clear();

  moveCommand = new ModelMoveLayoutChangeCommand(m_model, this);
  moveCommand->setStartRow(0);
  moveCommand->setEndRow(0);
  // At the beginning of the move, the target parent is row index 10
  moveCommand->setDestAncestors(QList<int>() << 10);
  // Because one of the parents 'older' siblings is moved, the target becomes row index 9
  moveCommand->setEndOfMoveDestAncestors(QList<int>() << 9);
  moveCommand->setDestRow(5);

  commandList << moveCommand;


  setCommand("move03", commandList);
  commandList.clear();

  moveCommand = new ModelMoveLayoutChangeCommand(m_model, this);
  moveCommand->setAncestorRowNumbers(QList<int>() << 9);
  moveCommand->setEndOfMoveSourceAncestors(QList<int>() << 10);
  moveCommand->setStartRow(5);
  moveCommand->setEndRow(5);
  moveCommand->setDestRow(0);

  commandList << moveCommand;

  setCommand("move04", commandList);
  commandList.clear();

  moveCommand = new ModelMoveLayoutChangeCommand(m_model, this);
  moveCommand->setStartRow(4);
  moveCommand->setEndRow(4);
  moveCommand->setDestAncestors(QList<int>() << 10);
  moveCommand->setEndOfMoveDestAncestors(QList<int>() << 9);
  moveCommand->setDestRow(0);

  commandList << moveCommand;

  setCommand("move05", commandList);
  commandList.clear();

  moveCommand = new ModelMoveLayoutChangeCommand(m_model, this);
  moveCommand->setAncestorRowNumbers(QList<int>() << 9);
  moveCommand->setEndOfMoveSourceAncestors(QList<int>() << 10);
  moveCommand->setStartRow(0);
  moveCommand->setEndRow(0);
  moveCommand->setDestRow(4);

  commandList << moveCommand;

  setCommand("move06", commandList);
  commandList.clear();

  // Give the top level item 10 'younger' siblings.
  ins = new ModelInsertCommand(m_model, this);
  ins->setStartRow(11);
  ins->setEndRow(20);

  commandList << ins;
  setCommand("insert04", commandList);
  commandList.clear();

  // Add more children to the top level item.
  // First 'older'
  ins = new ModelInsertCommand(m_model, this);
  ins->setAncestorRowNumbers(QList<int>() << 10 );
  ins->setStartRow(0);
  ins->setEndRow(9);

  commandList << ins;

  setCommand("insert05", commandList);
  commandList.clear();

  // Then younger
  ins = new ModelInsertCommand(m_model, this);
  ins->setAncestorRowNumbers(QList<int>() << 10 );
  ins->setStartRow(20);
  ins->setEndRow(29);

  commandList << ins;

  setCommand("insert06", commandList);
  commandList.clear();

  // Then somewhere in the middle.
  ins = new ModelInsertCommand(m_model, this);
  ins->setAncestorRowNumbers(QList<int>() << 10 );
  ins->setStartRow(10);
  ins->setEndRow(19);

  commandList << ins;

  setCommand("insert07", commandList);
  commandList.clear();

  // Add some more items for removing later.
  ins = new ModelInsertCommand(m_model, this);
  ins->setAncestorRowNumbers(QList<int>() << 10 << 5 );
  ins->setStartRow(0);
  ins->setEndRow(9);
  commandList << ins;
  ins = new ModelInsertCommand(m_model, this);
  ins->setAncestorRowNumbers(QList<int>() << 10 << 5 << 5 );
  ins->setStartRow(0);
  ins->setEndRow(9);
  commandList << ins;
  ins = new ModelInsertCommand(m_model, this);
  ins->setAncestorRowNumbers(QList<int>() << 10 << 5 << 5 << 5 );
  ins->setStartRow(0);
  ins->setEndRow(9);
  commandList << ins;
  ins = new ModelInsertCommand(m_model, this);
  ins->setAncestorRowNumbers(QList<int>() << 10 << 6 );
  ins->setStartRow(0);
  ins->setEndRow(9);
  commandList << ins;
  ins = new ModelInsertCommand(m_model, this);
  ins->setAncestorRowNumbers(QList<int>() << 10 << 7 );
  ins->setStartRow(0);
  ins->setEndRow(9);
  commandList << ins;

  setCommand("insert08", commandList);
  commandList.clear();

  // Insert a tree of items in one go.
  ins = new ModelInsertCommand(m_model, this);
  ins->setStartRow(0);
  ins->setAncestorRowNumbers(QList<int>() << 10 << 2 );
  ins->interpret(
    " - 1"
    " - 1"
    " - - 1"
    " - - 1"
    " - - - 1"
    " - - 1"
    " - 1"
    " - 1"
    " - 1"
    " - 1"
    " - - 1"
    " - - - 1"
    " - - 1"
    " - - - 1"
    " - 1"
    " - 1"
    " - 1"
    " - 1"
  );

  commandList << ins;

  setCommand("insert09", commandList);
  commandList.clear();

  ModelDataChangeCommand *dataChange = new ModelDataChangeCommand(m_model, this);

  dataChange->setAncestorRowNumbers(QList<int>() << 10 );
  dataChange->setStartRow(0);
  dataChange->setEndRow(0);

  commandList << dataChange;

  setCommand("change01", commandList);
  commandList.clear();

  dataChange = new ModelDataChangeCommand(m_model, this);
  dataChange->setAncestorRowNumbers(QList<int>() << 10);
  dataChange->setStartRow(4);
  dataChange->setEndRow(7);

  commandList << dataChange;

  setCommand("change02", commandList);
  commandList.clear();

  ModelRemoveCommand *rem;

  // Remove a single item without children.
  rem = new ModelRemoveCommand(m_model, this);
  rem->setAncestorRowNumbers(QList<int>() << 10 );
  rem->setStartRow(0);
  rem->setEndRow(0);

  commandList << rem;

  setCommand("remove01", commandList);
  commandList.clear();

  // Remove a single item with 10 children.
  rem = new ModelRemoveCommand(m_model, this);
  rem->setAncestorRowNumbers(QList<int>() << 10 );
  rem->setStartRow(6);
  rem->setEndRow(6);

  commandList << rem;

  setCommand("remove02", commandList);
  commandList.clear();

  // Remove a single item with no children from the top.
  rem = new ModelRemoveCommand(m_model, this);
  rem->setAncestorRowNumbers(QList<int>() << 10 << 5 );
  rem->setStartRow(0);
  rem->setEndRow(0);

  commandList << rem;

  setCommand("remove03", commandList);
  commandList.clear();

  // Remove a single second level item with no children from the bottom.
  rem = new ModelRemoveCommand(m_model, this);
  rem->setAncestorRowNumbers(QList<int>() << 10 << 5 );
  rem->setStartRow(8);
  rem->setEndRow(8);

  commandList << rem;

  setCommand("remove04", commandList);
  commandList.clear();

  // Remove a single second level item with no children from the middle.
  rem = new ModelRemoveCommand(m_model, this);
  rem->setAncestorRowNumbers(QList<int>() << 10 << 5 );
  rem->setStartRow(3);
  rem->setEndRow(3);

  commandList << rem;

  setCommand("remove05", commandList);
  commandList.clear();

  // clear the children of a second level item.
  rem = new ModelRemoveCommand(m_model, this);
  rem->setAncestorRowNumbers(QList<int>() << 10 << 5 );
  rem->setStartRow(0);
  rem->setEndRow(6);

  commandList << rem;

  setCommand("remove06", commandList);
  commandList.clear();

  // Clear a sub-tree;
  rem = new ModelRemoveCommand(m_model, this);
  rem->setAncestorRowNumbers(QList<int>() << 10 );
  rem->setStartRow(4);
  rem->setEndRow(4);

  commandList << rem;

  setCommand("remove07", commandList);
  commandList.clear();
}


void ModelCommander::setCommand(const QString &name, ModelChangeCommandList list)
{
  QPair<QString, ModelChangeCommandList> pair = qMakePair(name, list);

  m_commands.append(pair);
}