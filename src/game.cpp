/**
 * Copyright (C) 2012 by INdT
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * @author Rodrigo Goncalves de Oliveira <rodrigo.goncalves@openbossa.org>
 * @author Roger Felipe Zanoni da Silva <roger.zanoni@openbossa.org>
 */

#include "game.h"

#include "scene.h"

#include <QtQuick/QQuickWindow>

#include <QtGui/QCursor>

/*!
  \qmltype Game
  \instantiates Game
  \inqmlmodule Bacon2D 1.0
  \brief The root element containing one or more QuasiScene.

  The QuasiGame component is the root game component which
  contains one or more QuasiScene components.  Only one
  QuasiScene will be set as currentScene.

  Example usage:
  \qml
   import QtQuick 2.0
   import Bacon2D 1.0

   Game {
       id: game
       width: 800
       height: 600

       Scene {
           id: scene
           width: 300
           height: 300
       }
   }
   \endqml
*/
Game::Game(Bacon2DItem *parent)
    : Bacon2DItem(parent)
    , m_currentScene(0)
    , m_fps(30)
    , m_timerId(0)
{
    m_gameTime.start();
    m_timerId = startTimer(1000 / m_fps);
}

/*!
 * \qmlproperty Scene Game::currentScene
 * \brief The current Scene
 */
Scene *Game::currentScene() const
{
    return m_currentScene;
}

void Game::setCurrentScene(Scene *currentScene)
{
    if (m_currentScene == currentScene)
        return;

    if (m_currentScene) {
        m_currentScene->setRunning(false);
        m_currentScene->setVisible(false);
    }

    m_currentScene = currentScene;

    if (m_currentScene) {
        m_currentScene->setGame(this);

        m_currentScene->setParent(this);
        m_currentScene->setParentItem(this);

        m_currentScene->setRunning(true);
        m_currentScene->setVisible(true);
    }

    emit currentSceneChanged();
}

/*!
 * \qmlproperty int Game::fps
 * \brief The current frame rate
 */
int Game::fps() const
{
    return m_fps;
}

void Game::setFps(const int &fps)
{
    if (m_fps == fps)
        return;

    m_fps = fps;

    killTimer(m_timerId);
    m_gameTime.restart();
    m_timerId = startTimer(1000 / m_fps);

    emit fpsChanged();
}

void Game::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event)

    update();
}

void Game::update()
{
    long elapsedTime = m_gameTime.restart();
    if (m_currentScene)
        m_currentScene->update(elapsedTime);
}

/*!
 * \qmlproperty QPointF Game::mouse
 * \brief The point in the plane of the mouse pointer
 */
QPointF Game::mouse()
{
    return window()->mapFromGlobal(QCursor::pos());
}
