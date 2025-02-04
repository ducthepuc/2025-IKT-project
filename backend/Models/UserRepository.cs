//temporary instead of db
using Ecommerence.Core.Models;
using System.Collections.Generic;
using System.Linq;

namespace Ecommerence.Core.Repositories
{
    public class UserRepository
    {
        private static readonly List<User> _users = new List<User>
        {
            //this would come from registering
            new User { Id = 1, Email = "test@example.com", UserName = "testuser", Password = "password123" }
        };

        public User GetUserByUsername(string username)
        {
            return _users.FirstOrDefault(u => u.UserName == username);
        }

        public User GetUserByEmail(string email)
        {
            return _users.FirstOrDefault(u => u.Email == email);
        }

        public User GetUserById(int id)
        {
            return _users.FirstOrDefault(u => u.Id == id);
        }

        public void AddUser(User user)
        {
            _users.Add(user);
        }

        public bool DeleteUser(int id)
        {
            var user = GetUserById(id);
            if (user == null) return false;
            _users.Remove(user);
            return true;
        }
    }
    
    
}