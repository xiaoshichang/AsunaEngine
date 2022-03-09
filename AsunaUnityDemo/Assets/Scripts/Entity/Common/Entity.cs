
using System;

namespace AsunaEntity
{
    public class Entity
    {
        public Entity()
        {
            guid = Guid.NewGuid();
        }
    
        public Guid guid;
    }
}

