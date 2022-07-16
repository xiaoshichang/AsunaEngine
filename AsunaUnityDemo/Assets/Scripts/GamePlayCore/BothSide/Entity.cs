
using System;

namespace Asuna.GamePlayCore
{
    public class Entity
    {
        protected Entity()
        {
            guid = Guid.NewGuid();
        }

        private Guid guid;
    }
}

