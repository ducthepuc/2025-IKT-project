//temporary instead of db
using Ecommerence.Core.Models;
using Microsoft.AspNetCore.Http.HttpResults;
using Microsoft.AspNetCore.Mvc;
using System.Collections.Generic;
using System.Data.Common;
using System.Linq;

namespace Ecommerence.Core.Repositories
{
    public class ProductRepository
    {
        private static readonly List<Product> _products = new List<Product>
        {
            new Product{ Id = 1, Price = 100, Name = "Doobert", Description = "Loaf", Amount = 1, ImagePath="./doobert.png"},
            new Product{ Id = 2, Price = 100, Name = "Scoobert", Description = "Streak", Amount = 1, ImagePath="./scoobert.png"},
            new Product{ Id = 3, Price = 100, Name = "Goobert", Description = "Tiny", Amount = 1, ImagePath="./goobert.png"}
        };

        public List<Product> GetAllProducts(){
            return _products;
        }

        public Product GetProductByName(string name)
        {
            return _products.FirstOrDefault(p => p.Name == name);
        }

        public Product GetProductById(int id)
        {
            return _products.FirstOrDefault(p => p.Id == id);
        }

        public void AddProduct(Product product)
        {
            _products.Add(product);
        }
        public bool DeleteProduct(int id)
        {
            var product = GetProductById(id);
            if (product == null) return false;
            _products.Remove(product);
            return true;
        }
    }
    

}